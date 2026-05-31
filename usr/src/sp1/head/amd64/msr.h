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

#ifndef _MACHINE_MSR_H_
#define _MACHINE_MSR_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

#define IA32_SPEC_CTL       0x00000048
#define IA32_KERNEL_GS_BASE 0xC0000102
#define IA32_GS_BASE        0xC0000101
#define IA32_FS_BASE        0xC0000100
#define IA32_APIC_BASE_MSR  0x0000001B

#ifndef __ASSEMBLER__

__always_inline static inline uint64_t
md_rdmsr(uint32_t address)
{
    uint32_t lo, hi;

    __asmv(
        "rdmsr"
        : "=a" (lo), "=d" (hi)
        : "c" (address)
        : "memory"
    );

    return ((uint64_t)hi << 32) | lo;
}

__always_inline static inline void
md_wrmsr(uint32_t address, uint64_t val)
{
    uint32_t lo, hi;

    lo = val & 0xFFFFFFFF;
    hi = (val >> 32) & 0xFFFFFFFF;

    __asmv(
        "wrmsr"
        :
        : "a" (lo), "d" (hi),
          "c" (address)
        : "memory"
    );
}

#endif  /* !__ASSEMBLER__ */
#endif  /* !_MACHINE_MSR_H_ */
