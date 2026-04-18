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

void
trap_dispatch(struct trapframe *tf)
{
    const char *str;

    if (tf->vector >= NELEM(traptab)) {
        knot("fatal unknown vector %x\n", tf->vector);
    }

    knot("fatal %s\n", traptab[tf->vector]);
}
