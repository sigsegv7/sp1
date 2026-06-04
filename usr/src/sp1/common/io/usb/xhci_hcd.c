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
#include <os/driver.h>
#include <lib/printf.h>
#include <stdbool.h>

#define pr_trace(fmt, ...) \
    printf("xhci-hcd: " fmt, ##__VA_ARGS__)

#define HCD_CLASS       0x0C
#define HCD_SUBCLASS    0x03

/* Forward declarations */
static struct pci_driver self_pci;
static struct driver_desc driver_desc;

static status_t
xhci_driver_init(struct driver_desc *desc)
{
    status_t status;

    if (desc == NULL) {
        return STATUS_INVALID_PARAM;
    }

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

    if (dev == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* Just acccept any, but only one */
    if (once) {
        return STATUS_SUCCESS;
    }

    pr_trace("attached %04x:%04x\n", dev->device_id, dev->vendor_id);
    once = true;
    return STATUS_SUCCESS;
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
