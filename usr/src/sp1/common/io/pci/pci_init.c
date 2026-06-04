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
#include <sys/types.h>
#include <io/pci/pci.h>
#include <lib/printf.h>
#include <os/knot.h>
#include <io/pci/device.h>
#include <io/pci/pcireg.h>
#include <mu/pci.h>

#define pr_trace(fmt, ...) \
    printf("pci: " fmt, ##__VA_ARGS__)

/* Globals */
static struct pci_cam_hook cam;

/*
 * Add a device to the PCI device list
 *
 * TODO: Finish this
 */
static void
pci_device_add(struct pci_device *dev)
{
    uint16_t vendor_id;
    uint16_t device_id;

    vendor_id = cam.cam_readl(dev, PCIREG_VENDOR_ID) & 0xFFFF;
    device_id = cam.cam_readl(dev, PCIREG_DEVICE_ID) >> 16;

    dev->vendor_id = vendor_id;
    dev->device_id = device_id;
    pr_trace("detected %x:%x\n", device_id, vendor_id);
}

/*
 * Inspect a device on the PCI device to see if it is really
 * present.
 *
 * @dev: Device to inspect
 */
static void
pci_device_inspect(struct pci_device *dev)
{
    uint16_t vendor;
    uint8_t hdr_type;

    if (dev == NULL) {
        return;
    }

    vendor = cam.cam_readl(dev, PCIREG_VENDOR_ID) & 0xFFFF;
    if (vendor == 0xFFFF) {
        return;
    }

    /* Is this a single function device? */
    hdr_type = cam.cam_readl(dev, PCIREG_HDRTYPE) & 0xFF;
    if (!ISSET(hdr_type, BIT(7))) {
        pci_device_add(dev);
        return;
    }

    for (uint8_t func = 0; func < 8; ++func) {
        vendor = cam.cam_readl(dev, PCIREG_VENDOR_ID) & 0xFFFF;
        if (vendor == 0xFFFF) {
            continue;
        }

        dev->function = func;
        pci_device_add(dev);
    }
}

/*
 * Enumerate devices present on the PCI bus
 */
static void
pci_enumerate(void)
{
    struct pci_device dev;

    dev.function = 0;
    dev.device = 0;
    dev.bus = 0;

    for (uint8_t bus = 0; bus < 255; ++bus) {
        for (uint8_t devnum = 0; devnum < 32; ++devnum) {
            dev.bus = bus;
            dev.device = devnum;
            pci_device_inspect(&dev);
        }
    }
}

void
pci_init(void)
{
    status_t status;

    status = pci_init_cam(&cam);
    if (status != STATUS_SUCCESS) {
        knot("pci: failed to initialize cam\n");
    }

    pr_trace("initialized cam\n");
    pci_enumerate();
}
