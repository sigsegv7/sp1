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
#include <sys/cdefs.h>
#include <sys/status.h>
#include <mu/mmu.h>
#include <mm/vm.h>
#include <mm/physmem.h>
#include <machine/tlb.h>
#include <stdbool.h>
#include <string.h>

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

/* 57-bit linear addresses */
#define CR4_LA57 BIT(12)

/*
 * Represents valid pagemap levels
 */
typedef enum {
    PAGELVL_PML1,
    PAGELVL_PML2,
    PAGELVL_PML3,
    PAGELVL_PML4,
    PAGELVL_PML5
} pagelvl_t;

/*
 * Obtain page table flags from protection flags
 *
 * @prot: Protection flags to extract from
 */
static size_t
mmu_prot_to_pte(int prot)
{
    size_t pte_flags = PTE_P | PTE_NX;

    if (ISSET(prot, PROT_WRITE))
        pte_flags |= PTE_RW;
    if (ISSET(prot, PROT_EXEC))
        pte_flags &= ~PTE_NX;

    return pte_flags;
}

/*
 * Verify if a pagesize is valid
 *
 * @ps: Pagesize to verify
 */
static bool
mmu_ps_valid(pagelvl_t ps)
{
    switch (ps) {
    case PAGESIZE_4K:
        return true;
    }

    return false;
}

/*
 * Obtain the top-level in use for the current machine
 * configuration.
 */
static inline pagelvl_t
mmu_get_level(void)
{
    uint64_t cr4;

    __asmv(
        "mov %%cr4, %0"
        : "=r" (cr4)
        :
        : "memory"
    );

    return ISSET(cr4, CR4_LA57)
        ? PAGELVL_PML5
        : PAGELVL_PML4;
}

/*
 * This function extracts those cute 9 bit segments that
 * function as indices into pagemap levels.
 *
 * @vma:  Virtual memory address used as key
 * @lvl:  Level to extract
 */
static inline size_t
mmu_extract_index(uintptr_t vma, pagelvl_t lvl)
{
    switch (lvl) {
    case PAGELVL_PML1:
        return (vma >> 12) & 0x1FF;
    case PAGELVL_PML2:
        return (vma >> 21) & 0x1FF;
    case PAGELVL_PML3:
        return (vma >> 30) & 0x1FF;
    case PAGELVL_PML4:
        return (vma >> 39) & 0x1FF;
    case PAGELVL_PML5:
        return (vma >> 48) & 0x1FF;
    }

    return (size_t)-1;
}

/*
 * Extract a pagemap index
 *
 * @vfr: Virtual fuck region
 * @vma: Virtual memory address
 * @lvl: Pagemap level to extract
 * @alloc: If true, allocate new entries
 */
static uint64_t *
mmu_extract_level(struct mmu_vfr *vfr, uintptr_t vma, pagelvl_t lvl, bool alloc)
{
    uintptr_t *pmap, pma;
    void *tmp_p;
    size_t index;
    pagelvl_t cur_level;

    if (vfr == NULL || lvl > PAGELVL_PML5) {
        return NULL;
    }

    cur_level = mmu_get_level();
    pmap = pma_to_vma((vfr->cr3 & PTE_ADDR_MASK));

    while (cur_level > lvl) {
        index = mmu_extract_index(vma, cur_level);

        /* Is this entry present? */
        if (ISSET(pmap[index], PTE_P)) {
            pmap = pma_to_vma((pmap[index] & PTE_ADDR_MASK));
            --cur_level;
            continue;
        }

        if (!alloc) {
            return NULL;
        }

        pma = mm_physmem_alloc(1);
        if (pma == 0) {
            return NULL;
        }

        tmp_p = pma_to_vma(pma);
        memset(tmp_p, 0, 4096);
        pmap[index] = pma | (PTE_P | PTE_RW | PTE_US);

        pmap = tmp_p;
        --cur_level;
    }

    return pmap;
}

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

status_t
mu_mmu_map(struct mmu_vfr *vfr, uintptr_t vma, uintptr_t pma,
    int prot, pagesize_t ps)
{
    uintptr_t *tbl;
    size_t index, flags;

    if (vfr == NULL || !mmu_ps_valid(ps)) {
        return STATUS_INVALID_PARAM;
    }

    tbl = mmu_extract_level(vfr, vma, PAGELVL_PML1, true);
    if (tbl == NULL) {
        return STATUS_NO_MEMORY;
    }

    index = mmu_extract_index(vma, PAGELVL_PML1);
    flags = mmu_prot_to_pte(prot);

    tbl[index] = pma | flags;
    md_tlb_flush(vma);
    return STATUS_SUCCESS;

}
