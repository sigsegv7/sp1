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
#include <os/knot.h>
#include <mu/mmio.h>
#include <lib/printf.h>
#include <mm/vm.h>

/* IA32_APIC_BASE_MSR bits */
#define APIC_IS_BSP BIT(8)

/*
 * Used to print logs only on the bootstrap processor
 * to avoid spamming the console.
 */
#define bsp_trace(fmt, ...)  do {                   \
        uint64_t apic_base;                         \
                                                    \
        apic_base = md_rdmsr(IA32_APIC_BASE_MSR);   \
        if (ISSET(apic_base, APIC_IS_BSP)) {        \
            printf("lapic: " fmt, ##__VA_ARGS__);   \
        }                                           \
    } while (0);

/*
 * Read a value from the Local APIC register space
 *
 * @mcb:  Machine core block
 * @reg:  Register to read
 */
static uint64_t
lapic_read(struct mcb *mcb, uint16_t reg)
{
    uint32_t *reg_base;
    uint64_t v;

    if (mcb == NULL) {
        return 0;
    }

    if (!mcb->x2apic_enabled) {
        reg_base = PTR_OFFSET(mcb->lapic_mmio, reg);
        v = mmio_read32(reg_base);
    } else {
        knot("x2apic is currently unimplemented\n");
    }

    return v;
}

/*
 * Write a value to the Local APIC register space
 *
 * @mcb:  Machine core block
 * @reg:  Register to read
 * @v:    Value to write
 */
static uint64_t
lapic_write(struct mcb *mcb, uint16_t reg, uint64_t v)
{
    uint32_t *reg_base;

    if (mcb == NULL) {
        return 0;
    }

    if (!mcb->x2apic_enabled) {
        reg_base = PTR_OFFSET(mcb->lapic_mmio, reg);
        mmio_write32(reg_base, v);
    } else {
        knot("x2apic is currently unimplemented\n");
    }

    return v;
}

/*
 * Enable the Local APIC unit of the current processor
 */
static void
lapic_enable(struct mcb *mcb)
{
    uint64_t apic_base;
    uint64_t base_pma, svr;
    uint64_t version_reg;
    uint8_t max_lvt, version;
    const char *type;

    if (mcb == NULL) {
        knot("failed to enable LAPIC, mcb is NULL\n");
    }

    apic_base = md_rdmsr(IA32_APIC_BASE_MSR);
    base_pma = (apic_base >> 12) & 0xFFFFFF;
    mcb->lapic_mmio = pma_to_vma(base_pma);

    /* Hardware enable the chip */
    apic_base |= LAPIC_HW_ENABLE;
    md_wrmsr(IA32_APIC_BASE_MSR, apic_base);

    /* Software enable the chip */
    svr = lapic_read(mcb, LAPIC_SVR);
    svr |= LAPIC_SW_ENABLE;
    lapic_write(mcb, LAPIC_SVR, svr);

    /* Obtain version information  */
    version_reg = lapic_read(mcb, LAPIC_VERSION);
    max_lvt = ((version_reg >> 16) & 0xFF) - 1;
    version = version_reg & 0xFF;

    /* Obtain the Local APIC type */
    if (version < 0x10) {
        type = "discrete/82489DX";
    } else {
        type = "integrated";
    }

    bsp_trace("max_lvt : %d, type : %s\n", max_lvt, type);
}

status_t
md_lapic_init(struct cpu_info *ci)
{
    struct mcb *mcb;
    uint64_t apic_base;

    if (ci == NULL) {
        return STATUS_INVALID_PARAM;
    }

    mcb = &ci->mcb;
    lapic_enable(mcb);
    return STATUS_SUCCESS;
}
