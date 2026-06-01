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

#include <sys/types.h>
#include <os/knot.h>
#include <mm/kalloc.h>
#include <mm/tlsf.h>
#include <mm/physmem.h>
#include <mm/vm.h>
#include <stdbool.h>

#define PAGESIZE 4096  /* TODO: Do not hardcode this */
#define KALLOC_POOL_PAGES     (KALLOC_POOL_SZ / PAGESIZE)
#define KALLOC_POOL_SZ        0x400000  /* 4 MiB */

static tlsf_t tlsf_ctx; /* TODO: Make this per-core */
static uintptr_t pool = 0;
static void *pool_va = 0;
static bool is_init = false;

/*
 * Memory allocation
 */
void *
kalloc(size_t sz)
{
    void *tmp;

    if (!is_init) {
        return NULL;
    }

    tmp = tlsf_malloc(tlsf_ctx, sz);
    return tmp;
}

void *
krealloc(void *old_ptr, size_t newsize)
{
    void *tmp;

    tmp = tlsf_realloc(tlsf_ctx, old_ptr, newsize);
    return tmp;
}

/*
 * Memory deallocation
 */
void
kfree(void *ptr)
{
    tlsf_free(tlsf_ctx, ptr);
}

void
kalloc_init(void)
{
    /* Don't do it twice */
    if (is_init) {
        return;
    }

    pool = mm_physmem_alloc(KALLOC_POOL_PAGES);
    if (pool == 0) {
        knot("kalloc_init: could not create pool\n");
    }

    pool_va = pma_to_vma(pool);
    tlsf_ctx = tlsf_create_with_pool(pool_va, KALLOC_POOL_SZ);
    is_init = true;
}
