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
#include <sys/atomic.h>
#include <mu/mmu.h>
#include <os/task.h>
#include <os/cpulock.h>

static cpu_lock_t pid_lock;
static volatile size_t pid = 0;

status_t
task_init(uint32_t flags, struct task *res)
{
    struct mmu_vfr vfr;
    struct pcb *pcbp;
    status_t status;

    if (res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* Set the process ID */
    cpu_lock_acquire(&pid_lock, false);
    res->pid = atomic_inc_64(&pid);
    cpu_lock_release(&pid_lock);

    res->flags = flags;
    pcbp = &res->pcb;

    /* Fork the current VFR, we are the parent */
    mu_mmu_readvfr(&vfr);
    status = mu_mmu_forkvfr(&vfr, &pcbp->vfr);
    if (status != STATUS_SUCCESS) {
        goto fail;
    }

    return STATUS_SUCCESS;
fail:
    cpu_lock_acquire(&pid_lock, false);
    atomic_dec_64(&pid);
    cpu_lock_release(&pid_lock);
    return status;
}
