/* Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distribution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#ifndef _MACHINE_IDT_H_
#define _MACHINE_IDT_H_ 1

#ifndef __ASSEMBLER__
#include <sys/types.h>
#include <sys/cdefs.h>
#endif  /* !__ASSEMBLER__ */

#define IDT_INT_GATE    0x8E
#define IDT_TRAP_GATE   0x8F
#define IDT_USER_GATE   0xEE

#ifndef __ASSEMBLER__
/*
 * 64-bit interrupt gate descriptor
 */
struct idt_gate {
    uint16_t offset_low;
    uint16_t segment_sel;
    uint8_t ist   : 3;
    uint8_t zero  : 5;
    uint8_t type  : 4;
    uint8_t zero1 : 1;
    uint8_t dpl   : 2;
    uint8_t p     : 1;
    uint16_t offset_mid;
    uint32_t offset_high;
    uint32_t reserved;
};

/*
 * Points to the IDT base
 *
 * @limit: IDT limit
 * @base:  IDT base address
 */
struct __packed idtr {
    uint16_t  limit;
    uintptr_t base;
};

/*
 * Set an IDT gate desriptor
 *
 * @vector: Interrupt vector to set
 * @isr:    Interrupt service routine base
 * @type:   Gate descriptor type
 * @ist:    Interrupt stack table index
 */
void md_idt_set_gate(uint8_t vector, uintptr_t isr, uint8_t type, uint8_t ist);

/*
 * Load the IDT
 */
void md_idt_load(void);


/*
 * Set all interrupt vectors
 */
void md_set_vectors(void);

#endif  /* !__ASSEMBLER__ */
#endif  /* !_MACHINE_IDT_H_ */
