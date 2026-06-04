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
#include <mm/kalloc.h>
#include <stdbool.h>
#include <string.h>

#define pr_trace(fmt, ...) \
    printf("pci: " fmt, ##__VA_ARGS__)

/* Globals */
static struct pci_cam_hook cam;
static TAILQ_HEAD(, pci_device) dev_list;
static size_t dev_count = 0;

static void
pci_device_add(struct pci_device *dev)
{
    struct pci_device *dev_cpy;
    uint16_t vendor_id, device_id;
    uint32_t classrev;
    uint8_t class, subclass;

    vendor_id = cam.cam_readl(dev, PCIREG_VENDOR_ID) & 0xFFFF;
    device_id = cam.cam_readl(dev, PCIREG_DEVICE_ID) >> 16;
    classrev = cam.cam_readl(dev, PCIREG_CLASSREV);

    class = PCIREG_CLASS(classrev);
    subclass = PCIREG_SUBCLASS(classrev);

    dev->vendor_id = vendor_id;
    dev->device_id = device_id;
    dev->class_id = class;
    dev->subclass_id = subclass;

    pr_trace("detected %04x:%04x\n", device_id, vendor_id);
    dev_cpy = kalloc(sizeof(*dev_cpy));

    if (dev_cpy == NULL) {
        return;
    }

    memcpy(dev_cpy, dev, sizeof(*dev_cpy));
    TAILQ_INSERT_TAIL(&dev_list, dev_cpy, link);
    ++dev_count;
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

/*
 * Returns true if the device/vendor id matches the advertisement
 * packet
 */
static inline bool
pci_vd_match(struct pci_device *dev, struct pci_adv *adv)
{
    if (adv == NULL) {
        return NULL;
    }

    if (dev->vendor_id == adv->vendor_id &&
        dev->device_id == adv->device_id) {
        return true;
    }

    return false;
}

/*
 * Returns true if the class/subclass ID matches the advertisement
 * packet
 */
static inline bool
pci_sc_match(struct pci_device *dev, struct pci_adv *adv)
{
    if (adv == NULL) {
        return NULL;
    }

    if (dev->class_id == adv->class_id &&
        dev->subclass_id == adv->subclass_id) {
        return true;
    }

    return false;
}

struct pci_device *
pci_resolve_adv(struct pci_adv *adv)
{
    struct pci_device *dev;

    TAILQ_FOREACH(dev, &dev_list, link) {
        if (adv->ven_dev) {
            if ((pci_vd_match(dev, adv)))
                return dev;

            continue;
        }

        if (pci_sc_match(dev, adv)) {
            return dev;
        }
    }

    return NULL;
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
    TAILQ_INIT(&dev_list);

    pci_enumerate();
    pr_trace("detected %d device(s) inserted deep~\n", dev_count);
}
