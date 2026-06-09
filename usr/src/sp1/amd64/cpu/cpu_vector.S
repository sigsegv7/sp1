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

#include <machine/idt.h>
#include <machine/kfence.h>

    .macro set_trap vector, isr, ist
        mov $\vector, %rdi
        lea \isr(%rip), %rsi
        mov $IDT_TRAP_GATE, %rdx
        mov $\ist, %rcx
        call md_idt_set_gate
    .endm

    .macro set_uint vector isr ist
        mov $\vector, %rdi
        lea \isr(%rip), %rsi
        mov $IDT_USER_GATE, %rdx
        mov $\ist, %rcx
        call md_idt_set_gate
    .endm

    .macro push_trapframe vector
        .if \vector == 10 || \vector == 11 || \vector == 12 || \vector == 13 \
            || \vector == 14
            subq $8, %rsp
        .endif

        push %r15
        push %r14
        push %r13
        push %r12
        push %r11
        push %r10
        push %r9
        push %r8
        push %rbp
        push %rdi
        push %rsi
        push %rdx
        push %rcx
        push %rbx
        push %rax
        push $\vector
    .endm

    .macro pop_trapframe vector
        .if \vector == 10 || \vector == 11 || \vector == 12 || \vector == 13 \
            || \vector == 14
            addq $8, %rsp
        .endif

        add $8, %rsp
        pop %r15
        pop %r14
        pop %r13
        pop %r12
        pop %r11
        pop %r10
        pop %r9
        pop %r8
        pop %rbp
        pop %rdi
        pop %rsi
        pop %rdx
        pop %rcx
        pop %rbx
        pop %rax
    .endm

    .text
    .globl md_set_vectors
md_set_vectors:
    push %r12
    push %r13
    push %r14
    push %r15
    push %rbx
    push %rbp

    set_trap 0x00, diverr, 0
    set_trap 0x01, debug_except, 0
    set_trap 0x02, nmi, 0
    set_trap 0x03, breakpoint, 0
    set_trap 0x04, overflow, 0
    set_trap 0x05, bound_range, 0
    set_trap 0x06, invalid_tss, 0
    set_trap 0x07, no_coproc, 0
    set_trap 0x08, double_fault, 0
    set_trap 0x0A, invalid_tss, 0
    set_trap 0x0B, seg_np, 0
    set_trap 0x0C, ss_fault, 0
    set_trap 0x0D, gpf, 0
    set_trap 0x0E, page_fault, 0
    set_uint 0x80, syscall_entry, 0

    pop %rbp
    pop %rbx
    pop %r15
    pop %r14
    pop %r13
    pop %r12
    retq

    .align 8
diverr:
    KFENCE
    push_trapframe 0x00
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

debug_except:
    KFENCE
    push_trapframe 0x1
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

nmi:
    KFENCE
    push_trapframe 0x2
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

breakpoint:
    KFENCE
    push_trapframe 0x3
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

overflow:
    KFENCE
    push_trapframe 0x4
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

bound_range:
    KFENCE
    push_trapframe 0x5
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

invl_opc:
    KFENCE
    push_trapframe 0x6
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

no_coproc:
    KFENCE
    push_trapframe 0x7
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE
1:  cli
    hlt
    jmp 1b

double_fault:
    KFENCE_EC
    push_trapframe 0x8
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE_EC
1:  cli
    hlt
    jmp 1b

invalid_tss:
    KFENCE_EC
    push_trapframe 0xA
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE_EC
1:  cli
    hlt
    jmp 1b

seg_np:
    KFENCE_EC
    push_trapframe 0xB
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE_EC
1:  cli
    hlt
    jmp 1b

ss_fault:
    KFENCE_EC
    push_trapframe 0xC
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE_EC
1:  cli
    hlt
    jmp 1b

gpf:
    KFENCE_EC
    push_trapframe 0xD
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE_EC
1:  cli
    hlt
    jmp 1b

page_fault:
    KFENCE_EC
    push_trapframe 0xE
    mov %rsp, %rdi
    call trap_dispatch
    KFENCE_EC
1:  cli
    hlt
    jmp 1b
    hlt

    .globl lapic_tmr_isr
lapic_tmr_isr:
    KFENCE
    push_trapframe 0x81
    mov %rsp, %rdi
    call mu_task_switch
    call lapic_eoi
    pop_trapframe 0x81
    mov $9000, %rdi
    call lapic_timer_oneshot_us
    KFENCE
    iretq

    .globl syscall_entry
syscall_entry:
    KFENCE
    push_trapframe 0x80
    mov %rsp, %rdi
    call trap_dispatch
    pop_trapframe 0x80
    KFENCE
    iretq
