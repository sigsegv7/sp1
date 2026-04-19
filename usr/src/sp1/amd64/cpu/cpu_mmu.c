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
