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

#include <sys/mman.h>
#include <sys/types.h>
#include <sys/atomic.h>
#include <mu/mmu.h>
#include <mu/param.h>
#include <mm/vm.h>
#include <mm/physmem.h>
#include <os/task.h>
#include <os/cpulock.h>

static cpu_lock_t pid_lock;
static volatile size_t pid = 0;

static status_t
task_init_stack(struct task *task, uint32_t flags)
{
    struct mmu_vfr vfr;
    uintptr_t stack_pma;
    struct vm_map vm_map;
    status_t status;

    if (task == NULL) {
        return STATUS_INVALID_PARAM;
    }

    stack_pma = mm_physmem_alloc(BASE_STACK_SIZE / PAGESIZE);
    if (stack_pma == 0) {
        return STATUS_NO_MEMORY;
    }

    /* Just add higher half if kernel stack */
    if (!ISSET(flags, TASK_USER)) {
        task->stack_base = (uintptr_t)pma_to_vma(stack_pma);
        return STATUS_SUCCESS;
    }

    task->stack_base = stack_pma;
    task->stack_size = BASE_STACK_SIZE;
    mu_mmu_readvfr(&vfr);

    vm_map.ps = PAGESIZE_4K;
    vm_map.vma_base = stack_pma;
    vm_map.pma_base = stack_pma;
    vm_map.length = BASE_STACK_SIZE;
    status = mm_vm_map(&vfr, &vm_map, PROT_READ | PROT_WRITE);

    if (status != STATUS_SUCCESS) {
        return status;
    }

    return STATUS_SUCCESS;
}

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

    status = task_init_stack(res, flags);
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
