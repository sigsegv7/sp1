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

#ifndef _MACHINE_TSS_H_
#define _MACHINE_TSS_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

/*
 * 64-bit task state segment entry used to store
 * kernel stacks.
 */
struct __packed tss_entry {
    uint32_t reserved;
    uint32_t rsp0_low;
    uint32_t rsp0_high;
    uint32_t rsp1_low;
    uint32_t rsp1_high;
    uint32_t rsp2_low;
    uint32_t rsp2_high;
    uint32_t reserved1;
    uint32_t reserved2;
    uint32_t ist1_low;
    uint32_t ist1_high;
    uint32_t ist2_low;
    uint32_t ist2_high;
    uint32_t ist3_low;
    uint32_t ist3_high;
    uint32_t ist4_low;
    uint32_t ist4_high;
    uint32_t ist5_low;
    uint32_t ist5_high;
    uint32_t ist6_low;
    uint32_t ist6_high;
    uint32_t ist7_low;
    uint32_t ist7_high;
    uint32_t reserved3;
    uint32_t reserved4;
    uint16_t reserved5;
    uint16_t io_map;
};

/*
 * Represents a 64-bit TSS descriptor
 */
struct __packed tss_desc {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_mid7;
    uint8_t type : 4;
    uint8_t zero : 1;
    uint8_t dpl  : 2;
    uint8_t p    : 1;
    uint8_t limit_high : 4;
    uint8_t avl : 1;
    uint8_t zero1 : 2;
    uint8_t g : 1;
    uint8_t base_mid;
    uint32_t base_high;
    uint8_t reserved;
    uint8_t zero2 : 5;
    uint32_t reserved1;
};

#endif  /* !_MACHINE_TSS_H_ */
