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

#ifndef _MACHINE_GDT_H_
#define _MACHINE_GDT_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

/* Number of GDT entries */
#define GDT_ENTRY_COUNT 7

/* Kernel code/data */
#define GDT_KCODE  0x08
#define GDT_KDATA  0x10

/* User code/data */
#define GDT_UCODE  0x18
#define GDT_UDATA  0x20

/* Task state segment */
#define GDT_TSS    0x28
#define GDT_TSS_INDEX 0x05

struct __packed gdt_entry {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_mid;
    uint16_t attributes;
    uint8_t base_hi;
};

struct __packed gdtr {
    uint16_t limit;
    uintptr_t offset;
};

__always_inline static inline void
gdt_load(struct gdtr *gdtr)
{
    __asmv("lgdt %0\n"
           "push %1\n"              /* Push code segment selector */
           "lea 1f(%%rip), %%rax\n" /* Load label 1 address into RAX */
           "push %%rax\n"           /* Push return address (label 1) */
           "lretq\n"                /* Far return to update CS */
           "1:\n"
           "  mov %2, %%ax\n"       /* Load data segment selectors */
           "  mov %%ax, %%ds\n"
           "  mov %%ax, %%es\n"
           "  mov %%ax, %%fs\n"
           "  mov %%ax, %%gs\n"
           "  mov %%ax, %%ss\n"
           :
           : "m" (*gdtr), "i"(GDT_KCODE), "i"(GDT_KDATA)
           : "rax", "memory"
    );
}

#endif  /* !_MACHINE_GDT_H_ */
