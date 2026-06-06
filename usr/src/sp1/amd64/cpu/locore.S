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

    lea g_GDTR(%rip), %rdi
    call md_gdt_load

    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    retq

    .text
    .globl md_gdt_load
md_gdt_load:
    push %r12
    push %r13
    push %r14
    push %r15
    push %rbx
    push %rbp

    lgdt (%rdi)                 /* Load our own GDT */
    push $0x08                  /* Kernel CS */
    lea 1f(%rip), %rbx          /* Drop point */
    push %rbx                   /* RIP */
    lretq                       /* Flush prefetch + reload CS */
1:  mov $0x10, %ax              /* Kernel DS */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %ss
    mov %ax, %fs
    xor %ax, %ax
    mov %ax, %gs

    pop %rbp
    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    retq

    .globl md_cpu_irqen
md_cpu_irqen:
    pushf
    pop %rax
    test $1<<9, %rax
    jz bad
    mov $1, %rax
    retq
bad:
    mov $0, %rax
    retq

     .section .data
    .globl g_GDT
g_GDT:
.NULL:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b00000000
    .byte 0b00000000
    .byte 0x00
.CODE:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b00100000
    .byte 0x00
.DATA:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b00000000
    .byte 0x00
.UCODE:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b11111010
    .byte 0b10101111
    .byte 0x00
.UDATA:
    .word 0x0000
    .word 0x0000
    .byte 0x00
    .byte 0b11110010
    .byte 0b00000000
    .byte 0x00
.TSS:
    .quad 0x00
    .quad 0x00

    .globl g_GDTR
g_GDTR:
    .word g_GDTR - g_GDT
    .quad g_GDT
