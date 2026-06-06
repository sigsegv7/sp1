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
#include <io/pci/pci.h>
#include <io/pci/device.h>
#include <io/usb/xhcireg.h>
#include <io/usb/xhcivar.h>
#include <io/clkdev/ticker.h>
#include <os/cum.h>
#include <os/driver.h>
#include <mu/mmio.h>
#include <lib/printf.h>
#include <stdbool.h>
#include <mm/vm.h>

#define pr_trace(fmt, ...) \
    printf("xhci-hcd: " fmt, ##__VA_ARGS__)

#define HCD_CLASS        0x0C
#define HCD_SUBCLASS     0x03
#define HCD_TIMEOUT_MSEC 1000

/* Forward declarations */
static struct pci_driver self_pci;
static struct driver_desc driver_desc;

/* Globals */
static struct xhci_hc hc;
static struct clk_ticker *ticker;

/*
 * Poll a 32-bit xHC register
 *
 * @reg:        Register address to poll
 * @mask:       Mask to poll for
 * @pollset:    If true, poll until the mask is set
 */
static status_t
xhci_poll_reg(volatile uint32_t *reg, uint32_t mask, bool pollset)
{
    size_t count_goal;
    size_t count_start, count = 0;
    uint32_t v;

    count_start = ticker_get_count(ticker);
    count_goal = ticker_msec_delta(ticker, HCD_TIMEOUT_MSEC);

    while (count < count_start + count_goal) {
        v = mmio_read32(reg);

        if (pollset && ISSET(v, mask)) {
            return STATUS_SUCCESS;
        } else if (!pollset && !ISSET(v, mask)) {
            return STATUS_SUCCESS;
        }

        count = ticker_get_count(ticker);
    }

    return STATUS_TIMED_OUT;
}

/*
 * Halt a host controller
 *
 * @hc: Host controller to halt
 */
static status_t
xhci_hc_halt(struct xhci_hc *hc)
{
    struct xhci_opregs *opregs;
    uint32_t usbcmd, usbsts;
    status_t status;

    if (hc == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* Request the host controller to halt */
    opregs = hc->oper;
    usbcmd = mmio_read32(&opregs->usbcmd);
    usbcmd &= ~USBCMD_RUN;
    mmio_write32(&opregs->usbcmd, usbcmd);

    /* Wait for the host controller to halt */
    status = xhci_poll_reg(&opregs->usbsts, USBSTS_HCH, true);
    if (status != STATUS_SUCCESS) {
        pr_trace("error: timeout on USBSTS_HCH\n");
    }

    return STATUS_SUCCESS;
}

/*
 * Reset the host controller
 *
 * @hc: Host controller to reset
 */
static status_t
xhci_hc_reset(struct xhci_hc *hc)
{
    struct xhci_opregs *opregs;
    uint32_t usbcmd;
    status_t status;

    if (hc == NULL) {
        return STATUS_INVALID_PARAM;
    }

    opregs = hc->oper;
    usbcmd = mmio_read32(&opregs->usbcmd);
    usbcmd |= USBCMD_HCRST;
    mmio_write32(&opregs->usbcmd, usbcmd);

    /* Wait until the reset is complete */
    status = xhci_poll_reg(&opregs->usbcmd, USBCMD_HCRST, false);
    if (status != STATUS_SUCCESS) {
        pr_trace("error: timeout on USBCMD_HCRST\n");
        return status;
    }

    /* Wait until the controller is ready */
    status = xhci_poll_reg(&opregs->usbsts, USBSTS_CNR, false);
    if (status != STATUS_SUCCESS) {
        pr_trace("error: timeout on USBSTS_CNR\n");
    }

    return STATUS_SUCCESS;
}

/*
 * Initialize the xHCI host controller
 */
static status_t
xhci_hc_init(void *mmio_base)
{
    struct xhci_caps *cap_regs;
    struct xhci_opregs *oper_regs;
    size_t slots_enabled, len;
    uintptr_t pma;
    uint32_t config;
    void *tmp_p;
    status_t status;

    if (mmio_base == NULL) {
        return STATUS_INVALID_PARAM;
    }

    status = membox_init(&hc.membox);
    if (status != STATUS_SUCCESS) {
        pr_trace("error: failed to initialize memory box\n");
        return status;
    }

    cap_regs = mmio_base;
    oper_regs = PTR_OFFSET(cap_regs, cap_regs->caplength);

    hc.max_slots = XHCI_MAXSLOTS(cap_regs->hcsparams1);
    hc.max_ports = XHCI_MAXPORTS(cap_regs->hcsparams1);
    hc.caps = cap_regs;
    hc.oper = oper_regs;

    /* Some informational logging */
    pr_trace("hc.maxports : %d, hc.maxslots : %d\n",
        hc.max_ports, hc.max_slots);

    /* Halt the host controller before we reset */
    status = xhci_hc_halt(&hc);
    if (status != STATUS_SUCCESS) {
        pr_trace("error: failed to halt host controller\n");
        return status;
    }

    /* Now we can reset it */
    status = xhci_hc_reset(&hc);
    if (status != STATUS_SUCCESS) {
        pr_trace("error: failed to reset host controller\n");
        return status;
    }

    /* Enable all device slots */
    slots_enabled = hc.max_slots;
    config = mmio_read32(&oper_regs->config);
    config |= slots_enabled;
    mmio_write32(&oper_regs->config, config);

    len = sizeof(uintptr_t) * slots_enabled;
    tmp_p = membox_alloc(&hc.membox, len, MEM_TYPE_PHYSICAL);
    if (tmp_p == NULL) {
        pr_trace("error: failed to allocate dcbaa\n");
        return STATUS_NO_MEMORY;
    }

    /* Set the DCBAAP register */
    pma = vma_to_pma(tmp_p);
    mmio_write64(&oper_regs->dcbaa_ptr, pma);

    return STATUS_SUCCESS;
}

static status_t
xhci_driver_init(struct driver_desc *desc)
{
    status_t status;
    struct cum_resolve resolve;
    struct cum_object *tmr_obj;

    if (desc == NULL) {
        return STATUS_INVALID_PARAM;
    }

    resolve.lookup = NULL;
    resolve.path = "/clkdev/ticker0";
    resolve.flags = 0;
    status = cum_resolve(&resolve);

    if (status != STATUS_SUCCESS) {
        return status;
    }

    ticker = TICKER_DATA_FROM(resolve.lookup);
    status = pci_driver_register(&self_pci);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    return STATUS_SUCCESS;
}

static status_t
xhci_driver_kick(struct pci_device *dev)
{
    static bool once = false;
    void *mmio_space;

    if (dev == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* Just acccept any, but only one */
    if (once) {
        return STATUS_SUCCESS;
    }

    once = true;
    pr_trace("attached %04x:%04x\n", dev->device_id, dev->vendor_id);
    mmio_space = pci_resolve_bar(dev, 0);

    /* Verify the MMIO space */
    if (mmio_space == NULL) {
        pr_trace("error: failed to resolve BAR\n");
        return STATUS_NO_DEV;
    }

    return xhci_hc_init(mmio_space);
}

/* Advertisement table */
static struct pci_adv self_adv[1] = {
    PCI_CLASS_DEV(HCD_CLASS, HCD_SUBCLASS)
};

/* PCI driver descriptor */
static struct pci_driver self_pci = {
    .adv = self_adv,
    .adv_entries = NELEM(self_adv),
    .kick = xhci_driver_kick
};

/* Driver export */
__driver static struct driver_desc driver_desc = {
    .name = "xHCI",
    .author = "Chloe M.",
    .init = xhci_driver_init
};
