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
#include <mu/task.h>
#include <os/knot.h>
#include <lib/printf.h>
#include <machine/lapic.h>

void
mu_task_switch(struct trapframe *framep)
{
    if (__unlikely(framep == NULL)) {
        knot("task switch failure - framep == NULL\n");
    }

    printf("[*] sp1 is pre-alpha\n");
    printf("[*] knotting kernel...\n");
    knot("end of kernel reached - halting\n");
}

__no_return void
mu_task_mode(void)
{
    __asmv("sti");
    lapic_timer_oneshot_us(9000);
    for (;;) {
        __asmv("rep; nop");
    }
}
