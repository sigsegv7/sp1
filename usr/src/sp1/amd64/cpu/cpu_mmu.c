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

#include <sys/cdefs.h>
#include <mu/mmu.h>
#include <mm/vm.h>
#include <mm/physmem.h>

/*
 * Page-Table Entry (PTE) flags
 *
 * See Intel SDM Vol 3A, Section 4.5, Table 4-19
 */
#define PTE_ADDR_MASK   0x000FFFFFFFFFF000
#define PTE_P           BIT(0)        /* Present */
#define PTE_RW          BIT(1)        /* Writable */
#define PTE_US          BIT(2)        /* User r/w allowed */
#define PTE_PWT         BIT(3)        /* Page-level write-through */
#define PTE_PCD         BIT(4)        /* Page-level cache disable */
#define PTE_ACC         BIT(5)        /* Accessed */
#define PTE_DIRTY       BIT(6)        /* Dirty (written-to page) */
#define PTE_PS          BIT(7)        /* Page size */
#define PTE_GLOBAL      BIT(8)        /* Global; sticky */
#define PTE_NX          BIT(63)       /* Execute-disable */

void
mu_mmu_readvfr(struct mmu_vfr *res)
{
    if (res == NULL) {
        return;
    }

    __asmv(
        "mov %%cr3, %0"
        : "=r" (res->cr3)
        :
        : "memory"
    );
}

void
mu_mmu_writevfr(struct mmu_vfr *vfr)
{
    if (vfr == NULL) {
        return;
    }

    __asmv(
        "mov %0, %%cr3"
        :
        : "r" (vfr->cr3)
        : "memory"
    );
}

status_t
mu_mmu_forkvfr(struct mmu_vfr *vfr, struct mmu_vfr *res)
{
    uintptr_t dest;
    uintptr_t *dest_p, *src_p;

    if (vfr == NULL || res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if ((dest = mm_physmem_alloc(1)) == 0) {
        return STATUS_NO_MEMORY;
    }

    src_p = pma_to_vma((vfr->cr3 & PTE_ADDR_MASK));
    dest_p = pma_to_vma(dest);

    for (int i = 0; i < 512; ++i) {
        if (i < 256) {
            dest_p[i] = 0;
        } else {
            dest_p[i] = src_p[i];
        }
    }

    res->cr3 = dest;
    return STATUS_SUCCESS;
}
