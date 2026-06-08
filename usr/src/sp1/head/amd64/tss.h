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

struct cpu_info;

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

struct __packed tss_desc {
    uint16_t seglimit;
    uint16_t base_lo16;
    uint8_t base_mid8;
    uint8_t type        : 4;
    uint8_t zero        : 1;
    uint8_t dpl         : 2;
    uint8_t p           : 1;
    uint8_t seglimit_hi : 4;
    uint8_t avl         : 1;
    uint8_t unused      : 2;
    uint8_t g           : 1;
    uint8_t base_hi_mid8;
    uint32_t base_hi32;
    uint32_t reserved;
};

__always_inline static inline void
tss_load(void)
{
    __asmv(
        "str %ax\n"
        "mov $0x2B, %ax\n"
        "ltr %ax"
    );
}

/*
 * Write the data to the TSS
 *
 * @ci: Current processor information
 * @desc: TSS descriptor
 */
void write_tss(struct cpu_info *ci, struct tss_desc *desc);

#endif  /* !_MACHINE_TSS_H_ */
