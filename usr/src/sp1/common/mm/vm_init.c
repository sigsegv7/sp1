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

#include <os/knot.h>
#include <mu/mmu.h>
#include <lib/printf.h>

#define pr_trace(fmt, ...) \
    printf("vm: " fmt, ##__VA_ARGS__)

/* Kernel virtual fuck region */
static struct mmu_vfr kvfr;

void
mm_vm_init(void)
{
    struct mmu_vfr vfr;

    pr_trace("forking vfr\n");
    mu_mmu_readvfr(&vfr);
    if (mu_mmu_forkvfr(&vfr, &kvfr) != STATUS_SUCCESS) {
        knot("insufficient memory resources\n");
    }

    /* Update the current VFR */
    mu_mmu_writevfr(&kvfr);
    pr_trace("... OK\n");
}
