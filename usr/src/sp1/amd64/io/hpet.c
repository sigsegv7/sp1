/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distrubution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/cdefs.h>
#include <os/knot.h>
#include <os/cum.h>
#include <mu/mmio.h>
#include <io/clkdev/ticker.h>
#include <io/acpi/tables.h>
#include <io/acpi/acpi.h>
#include <machine/hpet.h>
#include <machine/hpetreg.h>
#include <mm/vm.h>
#include <mm/kalloc.h>
#include <lib/printf.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("hpet: " fmt, ##__VA_ARGS__)

/* MMIO base address */
static void *hpet_mmio64 = NULL;

/*
 * Read a 64-bit register from the HPET MMIO space
 */
static inline uint64_t
hpet_mmio_read(uint16_t offset)
{
    void *base;

    if (hpet_mmio64 == NULL) {
        return 0;
    }

    base = PTR_OFFSET(hpet_mmio64, offset);
    return mmio_read64(base);
}

static inline void
hpet_mmio_write(uint16_t offset, uint64_t v)
{
    void *base;

    if (hpet_mmio64 == NULL) {
        return;
    }

    base = PTR_OFFSET(hpet_mmio64, offset);
    mmio_write64(base, v);
}

static status_t
hpet_register(struct clk_ticker *ticker)
{
    struct clk_ticker *data;
    struct cum_object *cum_obj;
    status_t status;

    data = kalloc(sizeof(*data));
    if (data == NULL) {
        return STATUS_NO_MEMORY;
    }

    memcpy(data, ticker, sizeof(*data));
    status = cum_init_object(
        "ticker0",
        data,
        CUM_OBJECT_TIMER,
        &cum_obj
    );

    return cum_directory_add(
        clkdev_root,
        cum_obj
    );
}

static size_t
hpet_get_count(void)
{
    return hpet_mmio_read(HPET_MAIN_CNT);
}

static void
hpet_set_count(size_t v)
{
   hpet_mmio_write(HPET_MAIN_CNT, v);
}

status_t
md_hpet_init(void)
{
    struct ticker_ops *ops;
    struct clk_ticker ticker;
    struct acpi_hpet *hpet_desc;
    struct acpi_gas *gas;
    uint64_t gen_cap, gen_conf;
    uint32_t clk_period;
    uint8_t rev_id, tmr_cnt;
    status_t status;

    if ((hpet_desc = acpi_query("HPET")) == NULL) {
        knot("no presense of HPET detected");
    }

    /* Print some information before we init it */
    pr_trace("comparators :: %d\n", hpet_desc->comparator_count);
    pr_trace("rev         :: %d\n", hpet_desc->hardware_rev_id);
    pr_trace("vendor      :: %x\n", hpet_desc->pci_vendor_id);

    gas = &hpet_desc->gas;
    if (__unlikely(gas->address == 0)) {
        knot("got bad address in hpet gas\n");
    }

    hpet_mmio64 = pma_to_vma(gas->address);
    gen_cap = hpet_mmio_read(HPET_GENERAL_CAP);

    rev_id = gen_cap & 0xFF;
    tmr_cnt = (gen_cap >> 8) & 0x1F;
    clk_period = (gen_cap >> 32) & 0xFFFFFFFF;

    /* Spec states rev_id must not be zero */
    if (__unlikely(rev_id == 0)) {
        knot("got bad hpet revision id in general config\n");
    }

    /* Clock period must be valid */
    if (__unlikely(clk_period == 0 || clk_period > HPET_MAX_CLK_PERIOD)) {
        knot("got bad hpet clk period :: 0x%x\n", clk_period);
    }

    pr_trace("timer count :: %d\n", tmr_cnt);
    pr_trace("clk period  :: %d fs\n", clk_period);

    /* Enable timer with counter in known state */
    hpet_mmio_write(HPET_MAIN_CNT, 0);
    gen_conf = hpet_mmio_read(HPET_GENERAL_CONF);
    gen_conf |= HPET_GCONF_EN;
    hpet_mmio_write(HPET_GENERAL_CONF, gen_conf);

    memcpy(ticker.name, HPET_TICKER_NAME, sizeof(HPET_TICKER_NAME));
    ticker.period = clk_period;

    ops = &ticker.ops;
    ops->set_count = hpet_set_count;
    ops->get_count = hpet_get_count;
    status = hpet_register(&ticker);

    if (status != STATUS_SUCCESS) {
        return status;
    }

    return STATUS_SUCCESS;
}
