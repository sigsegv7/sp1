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
#include <io/pci/device.h>
#include <machine/pio.h>
#include <mu/pci.h>

/* PCI legacy CAM ports */
#define PCI_CONF_ADDR 0xCF8
#define PCI_CONF_DATA 0xCFC

/*
 * Obtain the PCI configuration address of a specific device
 * at a specific register offset
 */
static inline uint32_t
pci_conf_addr(struct pci_device *dev, uint32_t offset)
{
    return BIT(31)              |
        (offset & ~3)           |
        (dev->function << 8)    |
        (dev->device   << 11)   |
        (dev->bus      << 16);
}

/*
 * Write a value into the PCI configuration space
 *
 * @device: Device to write to
 * @offset: Register offset
 * @v:      Value to write
 */
static void
pci_writel(struct pci_device *device, uint8_t offset, uint32_t v)
{
    uint32_t address;

    if (device == NULL) {
        return;
    }

    address = pci_conf_addr(device, offset);
    md_outl(PCI_CONF_ADDR, address);
    md_outl(PCI_CONF_DATA, v);
}

/*
 * Read a value from the PCI configuration space
 *
 * @device: Device to read to
 * @offset: Register offset
 */
static uint32_t
pci_readl(struct pci_device *device, uint8_t offset)
{
    uint32_t address;

    if (device == NULL) {
        return 0;
    }

    address = pci_conf_addr(device, offset);
    md_outl(PCI_CONF_ADDR, address);
    return md_inl(PCI_CONF_DATA);
}

status_t
pci_init_cam(struct pci_cam_hook *hook_res)
{
    if (hook_res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    hook_res->cam_readl = pci_readl;
    hook_res->cam_writel = pci_writel;
    return STATUS_SUCCESS;
}
