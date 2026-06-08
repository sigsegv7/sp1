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

static scret_t
sys_syslog(struct syscall_args *args)
{
    if (args == NULL) {
        return 0;
    }

    /* TODO: CHECK THIS ADDRESS */
    printf("%s", (const char *)args->arg0);
    return 0;
}

scret_t(*g_sctab[])(struct syscall_args *) = {
    [SYS_NONE] = NULL,
    [SYS_SYSLOG] = sys_syslog
};

const size_t MAX_SYSCALLS = NELEM(g_sctab);
