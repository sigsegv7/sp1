/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distribution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#include <sys/param.h>
#include <sys/units.h>
#include <mm/physmem.h>
#include <mm/vm.h>
#include <mu/param.h>
#include <os/cpulock.h>
#include <lib/printf.h>
#include <os/bpt.h>
#include <string.h>

#define MEM_SCAN_START 0x00100000

#define pr_trace(fmt, ...) \
    printf("physseg: " fmt, ##__VA_ARGS__)

/* Use to convert type constants to strings */
#define MEM_TYPE(TYPE)          \
    ((TYPE) < NELEM(typetab))   \
        ? typetab[(TYPE)]       \
        : "invalid"

/*
 * Memory type constant to human readable
 * string table.
 */
static const char *typetab[] = {
    [MEMORY_USABLE]         = "usable",
    [MEMORY_RESERVED]       = "reserved",
    [MEMORY_ACPI_RECLAIM]   = "acpi reclaimable",
    [MEMORY_ACPI_NVS]       = "acpi nvs",
    [MEMORY_BAD]            = "bad*",
    [MEMORY_BOOTLOADER]     = "bootloader",
    [MEMORY_KERNEL]         = "syspaw",
    [MEMORY_FRAMEBUFFER]    = "framebuffer",
    [MEMORY_ACPI_TABLES]    = "acpi tables"
};

/* Memory stats */
static uintptr_t highest_usable;
static size_t mem_total = 0;
static size_t mem_usable = 0;
static size_t bitmap_size = 0;
static size_t last_idx = 0;
static size_t bitmap_free_start = 0;
static size_t highest_frame_idx = 0;
static uint8_t *bitmap;

/* Global lock */
static cpu_lock_t lock;

static inline void
physmem_print_size(const char *title, size_t len)
{
    if (title == NULL) {
        return;
    }

    if (len >= UNIT_GIB) {
        pr_trace("... %d GiB %s\n", len / UNIT_GIB, title);
    } else if (len >= UNIT_MIB) {
        pr_trace("... %d MiB %s\n", len / UNIT_MIB, title);
    } else {
        pr_trace("... %d bytes %s\n", len, title);
    }
}

/*
 * Populate physical memory bitmap.
 */
static void
physmem_bitmap_fill(void)
{
    struct mem_entry entry;
    status_t status;

    for (size_t i = 0;; ++i) {
        status = bpt_mem_entry_i(i, &entry);

        /* Are there no more entries? */
        if (status != STATUS_SUCCESS) {
            break;
        }

        if (entry.base < MEM_SCAN_START) {
            continue;
        }

        if (bitmap_free_start == 0) {
            bitmap_free_start = entry.base / PAGESIZE;
        }

        for (size_t j = 0; j < entry.length; j += PAGESIZE) {
            CLRBIT(bitmap, (entry.base + j) / PAGESIZE);
        }
    }
}

/*
 * Allocate memory used for the bitmap
 */
static void
physmem_bitmap_alloc(void)
{
    struct mem_entry entry;
    status_t status;

    for (size_t i = 0;; ++i) {
        status = bpt_mem_entry_i(i, &entry);

        /* Are there no more entries? */
        if (status != STATUS_SUCCESS) {
            break;
        }

        if (entry.type != MEMORY_USABLE) {
            continue;
        }

        if (entry.length < bitmap_size) {
            continue;
        }

        bitmap = pma_to_vma(entry.length);
        memset(bitmap, 0xFFFFFFFF, bitmap_size);
        break;
    }

    physmem_bitmap_fill();
}

/*
 * Allocate page frames.
 *
 * @count: Number of frames to allocate.
 */
static uintptr_t
__physmem_alloc_frame(size_t count)
{
    size_t frames = 0;
    ssize_t idx = -1;
    uintptr_t ret = 0;

    for (size_t i = last_idx; i < highest_frame_idx; ++i) {
        if (!TESTBIT(bitmap, i)) {
            if (idx < 0)
                idx = i;
            if (++frames >= count)
                break;

            continue;
        }

        idx = -1;
        frames = 0;
    }

    if (idx < 0 || frames != count) {
        ret = 0;
        goto done;
    }

    for (size_t i = idx; i < idx + count; ++i) {
        SETBIT(bitmap, i);
    }
    ret = idx * PAGESIZE;
    last_idx = idx;
    memset(pma_to_vma(ret), 0, count * PAGESIZE);
done:
    return ret;
}

uintptr_t
mm_physmem_alloc(size_t count)
{
   uintptr_t ret;

    if (count == 0) {
        return 0;
    }

    cpu_lock_acquire(&lock, true);
    if ((ret = __physmem_alloc_frame(count)) == 0) {
        last_idx = 0;
        ret = __physmem_alloc_frame(count);
    }

    cpu_lock_release(&lock);
    return ret;
}

/*
 * Central frame freeing routine
 */
void
mm_physmem_free(uintptr_t base, size_t count)
{
    size_t stop_at = base + (count * PAGESIZE);

    cpu_lock_acquire(&lock, true);
    base = ALIGN_UP(base, PAGESIZE);
    for (uintptr_t p = base; p < stop_at; p += PAGESIZE) {
        CLRBIT(bitmap, p / PAGESIZE);
    }

    cpu_lock_release(&lock);
}

/*
 * Begin scanning physical memory for usable entries
 * and other information
 */
static void
physmem_probe(void)
{
    struct mem_entry entry;
    status_t status;

    for (size_t i = 0;; ++i) {
        status = bpt_mem_entry_i(i, &entry);

        /* Are there no more entries? */
        if (status != STATUS_SUCCESS) {
            break;
        }

        /* Print this entry */
        pr_trace(
            "%p ... %p : %s\n",
            entry.base,
            entry.base + entry.length,
            MEM_TYPE(entry.type)
        );

        mem_total += entry.length;

        /* Skip non-usable memory from here on out */
        if (entry.type != MEMORY_USABLE) {
            continue;
        }

        if ((entry.base + entry.length) > highest_usable) {
            highest_usable = ALIGN_UP(entry.base + entry.length, PAGESIZE);
        }

        mem_usable += entry.length;
    }

    /* Obtain the size of the bitmap */
    highest_frame_idx = highest_usable / PAGESIZE;
    bitmap_size = highest_frame_idx / 8;

    /* Print memory stats */
    pr_trace("begin stats\n");
    physmem_print_size("usable", mem_usable);
    physmem_print_size("total", mem_total);

    /* Allocate a new bitmap */
    physmem_bitmap_alloc();
}

void
mm_physmem_init(void)
{
    /* Probe the physical memory */
    pr_trace("sniffing out physical memory...\n");
    physmem_probe();
}
