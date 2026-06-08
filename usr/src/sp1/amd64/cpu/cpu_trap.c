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

#include <sys/syscall.h>
#include <sys/param.h>
#include <os/knot.h>
#include <machine/frame.h>
#include <machine/trap.h>

/* Used to convert trap codes to strings */
static const char *traptab[] = {
    [TRAP_DIVERR] = "divide error",
    [TRAP_DBG]    = "debug exception",
    [TRAP_NMI]    = "non-maskable interrupt",
    [TRAP_BP]     = "breakpoint",
    [TRAP_OF]     = "overflow",
    [TRAP_BR]     = "bound range exceeded",
    [TRAP_UD]     = "undefined opcode",
    [TRAP_NM]     = "no math coprocessor",
    [TRAP_DF]     = "double fault",
    [TRAP_CPR]    = "reserved exception",
    [TRAP_TS]     = "invalid TSS",
    [TRAP_NP]     = "segment not present",
    [TRAP_SS]     = "stack segment fault",
    [TRAP_GP]     = "general protection fault",
    [TRAP_PF]     = "page fault"
};

static void
trap_syscall(struct trapframe *tf)
{
    struct syscall_args scargs = {
        .arg0 = tf->rdi,
        .arg1 = tf->rsi,
        .arg2 = tf->rdx,
        .arg3 = tf->r10,
        .arg4 = tf->r9,
        .arg5 = tf->r8,
        .tf = tf
    };

    if (tf->rax >= MAX_SYSCALLS || tf->rax == 0) {
        return;
    }

    tf->rax = g_sctab[tf->rax](&scargs);
}

void
trap_dispatch(struct trapframe *tf)
{
    const char *str;

    if (tf->vector == 0x80) {
        trap_syscall(tf);
        return;
    }

    if (tf->vector >= NELEM(traptab)) {
        knot("fatal unknown vector %x\n", tf->vector);
    }

    knot("fatal %s\n", traptab[tf->vector]);
}
