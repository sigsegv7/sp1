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
#include <sys/syscall.h>
#include <lib/printf.h>
#include <os/task.h>
#include <mu/cpu.h>

static scret_t
sys_syslog(struct syscall_args *args)
{
    struct cpu_info *ci;
    struct task *cur_task;
    bool range_good;
    const char *p;

    if (args == NULL) {
        return 0;
    }

    if ((ci = mu_this_cpu()) == NULL) {
        return 0;
    }

    cur_task = ci->cur_task;
    range_good = elf_vrange_good(&cur_task->x_snapshot, args->arg0, args->arg1);

    if (range_good) {
        p = (const char *)args->arg0;
        for (int i = 0; i < args->arg1; ++i) {
            printf("%c", p[i]);
        }
    }

    return 0;
}

scret_t(*g_sctab[])(struct syscall_args *) = {
    [SYS_NONE] = NULL,
    [SYS_SYSLOG] = sys_syslog
};

const size_t MAX_SYSCALLS = NELEM(g_sctab);
