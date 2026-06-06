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
#include <os/cpulock.h>
#include <mu/prim.h>

void
cpu_lock_acquire(cpu_lock_t *lock, bool irqmut)
{
    bool irq_en = md_cpu_irqen();

    if (lock == NULL) {
        return;
    }

    if (irqmut && irq_en) {
        md_cpu_irqoff();
    }

    while (mu_cpu_aswap(lock, 1))  {
        mu_cpu_spinwait();
    }

    if (irqmut && irq_en) {
        md_cpu_irqon();
    }
}

void
cpu_lock_release(cpu_lock_t *lock)
{
    if (lock == NULL) {
        return;
    }

    mu_cpu_aswap(lock, 0);
}
