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

    .text
    .globl knot
knot:
    cli     // Ensure interrupts are disabled
    cld     // Ensure the direction flag is clear

    //
    // On a multi-processor system we really want to ensure that
    // only the current processor core gets to fuck and knot the
    // kernel. Atomically acquire the MP lock to lockout other cores
    // out.
    //
    mov $1, %rax                // Obtain lock
    xchg %rax, __mp_lock        // Swap it atomically
    or %rax, %rax               // Are we another processor?
    jnz .lockout                // Yes, enter lockout

    //
    // Cumdrunk kernels cannot be trusted, we must update the stack
    // to a known address to avoid clobbering memory we don't wanna
    // touch. We also want to ensure that any partial memory transactions
    // that have not 100% finished to be finished before we enter the main
    // panic handler. This might not be fully needed but it is a precaution
    // to be taken.
    //
    lea stack_top(%rip), %rsp
    mfence
    jmp __knot
.lockout:
    hlt
    jmp .lockout

    .section .data
__mp_lock: .quad 0

    .section .bss
stack_base: .skip 4096, 0
stack_top:
