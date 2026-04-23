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

#include <sys/units.h>
#include <sys/param.h>
#include <lib/printf.h>
#include <mu/mmu.h>
#include <mu/param.h>
#include <mm/vm.h>

#define pr_trace(fmt, ...) \
    printf("vm_map: " fmt, ##__VA_ARGS__)

/* Used to safely convert pagesize constants */
#define GRAN(ps)        \
    ((ps) <= PAGESIZE_1G)   \
        ? pstab[(ps)]       \
        : pstab[PAGESIZE_4K]

/* Pagesize constant to length table */
static size_t pstab[] = {
    [PAGESIZE_4K] = 0x1000,
    [PAGESIZE_2M] = UNIT_MIB * 2,
    [PAGESIZE_1G] = UNIT_GIB
};

status_t
mm_vm_map(struct mmu_vfr *vfr, struct vm_map *mapping, int prot)
{
    size_t len, gran;
    uintptr_t vma, pma;
    status_t status;

    if (vfr == NULL || mapping == NULL) {
        return STATUS_INVALID_PARAM;
    }

    gran = GRAN(mapping->ps);
    len = mapping->length;

    vma = ALIGN_DOWN(mapping->vma_base, gran);
    pma = ALIGN_DOWN(mapping->pma_base, gran);
    len = ALIGN_UP(len + (len & (gran - 1)), gran);

    for (size_t i = 0; i < len; i += gran) {
        status = mu_mmu_map(vfr, vma + i, pma + i, prot, mapping->ps);

        /*
         * TODO: We need to clean up here instead of leaving a big hole
         *       when this fails.
         */
        if (status != STATUS_SUCCESS) {
            pr_trace("mapping failed, leaked %d bytes\n", len);
            return status;
        }
    }

    return STATUS_SUCCESS;
}
