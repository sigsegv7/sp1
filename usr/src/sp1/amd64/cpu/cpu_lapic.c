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
#include <sys/cdefs.h>
#include <machine/lapic.h>
#include <machine/lapicreg.h>
#include <machine/msr.h>
#include <lib/printf.h>

/* IA32_APIC_BASE_MSR bits */
#define APIC_IS_BSP BIT(8)

#define bsp_trace(fmt, ...)  do {                   \
        uint64_t apic_base;                         \
                                                    \
        apic_base = md_rdmsr(IA32_APIC_BASE_MSR);   \
        if (ISSET(apic_base, APIC_IS_BSP)) {        \
            printf("lapic: " fmt, ##__VA_ARGS__);   \
        }                                           \
    } while (0);

status_t
md_lapic_init(void)
{
    uint64_t apic_base;
    uint64_t base_pma;

    apic_base = md_rdmsr(IA32_APIC_BASE_MSR);
    base_pma = (apic_base >> 12) & 0xFFFFFF;

    bsp_trace("mmio.pma=%p\n", base_pma)
    return STATUS_SUCCESS;
}
