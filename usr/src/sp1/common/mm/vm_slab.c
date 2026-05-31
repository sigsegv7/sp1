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
#include <mm/slab.h>
#include <mm/physmem.h>
#include <mm/vm.h>
#include <lib/printf.h>
#include <os/knot.h>

#define pr_trace(fmt, ...) \
    printf("slab: " fmt, ##__VA_ARGS__)

/* Obtains the storage stride for the granularity */
#define SLAB_STRIDE(level) \
    (1 << ((level) + 5))

/*
 * Convert an allocation size to a specific slab list
 *
 * @mag:  Magazine host of slab lists
 * @sz:   Requested allocation size
 */
static struct slab *
slab_get_level(struct slab_mag *mag, size_t sz)
{
    size_t n = sz, level = 0;

    if (mag == NULL) {
        return NULL;
    }

    /* Fix up the size if it is too small */
    if (sz < SLAB_STRIDE(0)) {
        sz = SLAB_STRIDE(0);
    }

    sz = ALIGN_UP(sz, SLAB_STRIDE(0));

    /* (log2(size) - 5) => index */
    while (n != 0) {
        n >>= 1;
        ++level;
    }

    /* Subtract away the minimum size */
    if (level > 5) {
        level -= 5;
    }

    /* Should not happen but if it does, handle it gracefully */
    if (__unlikely(level >= SLAB_MAG_ENTRIES)) {
        level = SLAB_MAG_ENTRIES - 1;
    }

    return &mag->slab[level];
}

/*
 * Initialize a slab
 *
 * @slab: Slab to initialize
 */
static status_t
slab_init(struct slab *slab)
{
    uintptr_t pma;

    if (slab == NULL) {
        return STATUS_INVALID_PARAM;
    }

    pma = mm_physmem_alloc(PAGES_PER_SLAB);
    if (pma == 0) {
        return STATUS_NO_MEMORY;
    }

    slab->pool = pma_to_vma(pma);
    slab->bitmap = 0;       /* All free */
    slab->next = NULL;
    return STATUS_SUCCESS;
}

void *
slab_allocate(struct slab_mag *mag, size_t sz)
{
    if (mag == NULL || sz == 0) {
        return NULL;
    }

    sz = ALIGN_UP(sz, 2);
}

status_t
slab_mag_init(struct slab_mag *mag)
{
    if (mag == NULL) {
        return STATUS_INVALID_PARAM;
    }

    for (int i = 0; i < SLAB_MAG_ENTRIES; ++i) {
        if (slab_init(&mag->slab[i]) != STATUS_SUCCESS) {
            knot("failed to initialize magazine\n");
        }
    }

    mag->slabs_used = 0;
    mag->slabs_free = SLAB_MAG_ENTRIES;
    return STATUS_SUCCESS;
}
