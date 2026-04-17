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
    .globl _start
    .extern main
_start:
    cli
    cld

    xor %rbp, %rbp
    call main

    cli
1:  hlt
    jmp 1b

    .globl md_cpu_init
    .set IA32_EFER, 0xC0000080
md_cpu_init:
    push %r12
    push %r13
    push %r14
    push %r15
    push %rbx

    mov $IA32_EFER, %ecx    // IA32_EFER
    rdmsr                   // -> EDX:EAX
    or $1<<11, %eax         // Set NXE
    wrmsr                   // Write it back

    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    retq
