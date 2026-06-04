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

#include <sys/types.h>
#include <sys/param.h>
#include <sys/cdefs.h>
#include <machine/lapic.h>
#include <machine/lapicreg.h>
#include <machine/cpuid.h>
#include <machine/idt.h>
#include <machine/msr.h>
#include <os/knot.h>
#include <os/cum.h>
#include <io/clkdev/ticker.h>
#include <mu/mmio.h>
#include <lib/printf.h>
#include <mm/vm.h>
#include <stdbool.h>

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

extern void lapic_tmr_isr(void *sf);

/*
 * Returns true if the processor contains a Local APIC
 * unit.
 */
static inline bool
lapic_is_present(void)
{
    uint64_t unused, edx;

    __cpuid(0x01, unused, unused, unused, edx);
    return ISSET(edx, BIT(9)) != 0;
}

/*
 * Returns true if the Local APIC supports x2APIC mode
 */
static inline bool
x2apic_is_present(void)
{
    uint32_t ecx, unused;

    __cpuid(0x01, unused, unused, ecx, unused);
    return ISSET(ecx, BIT(21)) != 0;
}

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
        reg >>= 4;
        v = md_rdmsr(x2APIC_MSR_BASE + reg);
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
static void
lapic_write(struct mcb *mcb, uint16_t reg, uint64_t v)
{
    uint32_t *reg_base;

    if (mcb == NULL) {
        return;
    }

    if (!mcb->x2apic_enabled) {
        reg_base = PTR_OFFSET(mcb->lapic_mmio, reg);
        mmio_write32(reg_base, v);
    } else {
        reg >>= 4;
        md_wrmsr(x2APIC_MSR_BASE + reg, v);
    }
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
    apic_base |= mcb->x2apic_enabled << x2APIC_ENABLE_SHIFT;
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

/*
 * Put the Local APIC timer in a disabled state
 */
static void
lapic_timer_stop(struct mcb *mcb)
{
    lapic_write(mcb, LAPIC_LVT_TMR, LAPIC_LVT_MASK);
    lapic_write(mcb, LAPIC_INIT_CNT, 0);
}

/*
 * Calibrate the Local APIC timer
 */
static size_t
lapic_timer_init(struct mcb *mcb)
{
    const uint16_t MAX_SAMPLES = 0xFFFF;
    struct cum_resolve resolve;
    struct cum_object *tmr_obj;
    struct clk_ticker *ticker;
    status_t status;
    size_t ticks_start, ticks_end;
    size_t ticks_total, freq, unit;
    size_t ref_freq;

    if (mcb == NULL) {
        return 0;
    }

    resolve.lookup = NULL;
    resolve.path = "/clkdev/ticker0";
    resolve.flags = 0;
    status = cum_resolve(&resolve);

    if (status != STATUS_SUCCESS) {
        knot("lapic: could not resolve ticker0\n");
    }

    ticker = TICKER_DATA_FROM(resolve.lookup);
    lapic_timer_stop(mcb);

    bsp_trace("calibrating per-core timers...\n");
    ticks_start = ticker_get_count(ticker);
    lapic_write(mcb, LAPIC_INIT_CNT, MAX_SAMPLES);

    while (lapic_read(mcb, LAPIC_CUR_CNT) != 0);

    ticks_end = ticker_get_count(ticker);
    ticks_total = ticks_end - ticks_start;

    unit = ticker_unit(ticker);
    ref_freq = 1000000000000000ULL / ticker->period;
    freq = (MAX_SAMPLES * ref_freq) / ticks_total;

    mcb->lapic_tmr_freq = freq;
    return freq;
}

/*
 * Starts the Local APIC countdown timer...
 *
 * @mcb:  Machine core block
 * @mask: True to mask timer.
 * @mode: Timer mode.
 * @count: Count to start at.
 */
static inline void
lapic_timer_start(struct mcb *mcb, bool mask, uint8_t mode, uint32_t count)
{
    uint32_t tmp;

    if (mcb == NULL) {
        return;
    }

    tmp = (mode << 17) | (mask << 16) | LAPIC_TIMER_INTVEC;
    lapic_write(mcb, LAPIC_LVT_TMR, tmp);
    lapic_write(mcb, LAPIC_DCR, 0);
    lapic_write(mcb, LAPIC_INIT_CNT, count);
}

/*
 * Start Local APIC timer oneshot with number
 * of ticks to count down from.
 *
 * @mcb:  Machine core block
 * @mask: If `true', timer will be masked, `count' should be 0.
 * @count: Number of ticks.
 */
static void
lapic_timer_oneshot(struct mcb *mcb, bool mask, uint32_t count)
{
    if (mcb == NULL) {
        return;
    }

    lapic_timer_start(mcb, mask, LVT_TMR_ONESHOT, count);
}

/*
 * Start Local APIC timer oneshot in microseconds.
 *
 * @mcb: Machine core block
 * @us: Microseconds.
 */
static void
lapic_timer_oneshot_us(struct mcb *mcb, size_t usec)
{
    uint64_t ticks;

    if (mcb == NULL) {
        return;
    }

    ticks = usec * (mcb->lapic_tmr_freq / 1000000);
    lapic_timer_oneshot(mcb, false, ticks);
}

status_t
md_lapic_init(struct cpu_info *ci)
{
    struct mcb *mcb;
    uint64_t apic_base;

    if (ci == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* XXX: SP1 relies on Local APIC */
    if (!lapic_is_present()) {
        knot("processor does not have Local APIC unit");
    }

    mcb = &ci->mcb;
    mcb->x2apic_enabled = x2apic_is_present();
    lapic_enable(mcb);

    md_idt_set_gate(
        LAPIC_TIMER_INTVEC,
        (uintptr_t)lapic_tmr_isr,
        IDT_INT_GATE,
        0
    );

    lapic_timer_init(mcb);
    return STATUS_SUCCESS;
}
