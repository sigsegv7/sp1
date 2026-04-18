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
#include <mu/param.h>
#include <lib/printf.h>
#include <os/bpt.h>

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

    /* Print memory stats */
    pr_trace("begin stats\n");
    physmem_print_size("usable", mem_usable);
    physmem_print_size("total", mem_total);
}

void
mm_physmem_init(void)
{
    /* Probe the physical memory */
    pr_trace("sniffing out physical memory...\n");
    physmem_probe();
}
