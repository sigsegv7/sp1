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

#ifndef _PCI_PCI_H_
#define _PCI_PCI_H_ 1

#include <sys/status.h>
#include <sys/types.h>
#include <io/pci/device.h>

/* Vendor/device pair */
#define PCI_VEN_DEV(VENDOR_ID, DEVICE_ID)   \
    {                                       \
        .vendor_id = (VENDOR_ID),           \
        .device_id = (DEVICE_ID),           \
        .ven_dev = 1                        \
    }

/* Class/subclass pair */
#define PCI_CLASS_DEV(CLASS, SUBCLASS)      \
    {                                       \
        .class_id       = (CLASS),          \
        .subclass_id    = (SUBCLASS),       \
        .ven_dev        = 0                 \
    }

/*
 * Represents a PCI device advertisement
 *
 * @vendor_id:   Device vendor ID
 * @device_id:   Device ID
 * @class_id:    Device class
 * @subclass_id: Subclass ID
 * @ven_dev:     If set, using vendor/device pair
 */
struct pci_adv {
    uint16_t vendor_id;
    uint16_t device_id;
    uint16_t class_id;
    uint16_t subclass_id;
    uint8_t ven_dev : 1;
};

/*
 * Represents a PCI driver
 *
 * @adv:            Advertisement packet
 * @adv_entries:    Advertisement entries
 * @kick:           Initializes the hardware associated with the PCI device driver
 */
struct pci_driver {
    struct pci_adv *adv;
    size_t adv_entries;
    status_t(*kick)(struct pci_device *dev);
};

/*
 * Resolve a PCI device via its advertisement structure
 *
 * @adv: Adverisement structure to lookup
 *
 * Returns the PCI device on success, otherwise NULL on
 * failure.
 */
struct pci_device *pci_resolve_adv(struct pci_adv *adv);

/*
 * Register a PCI device driver
 *
 * @driver: Driver to advertise
 */
status_t pci_driver_register(struct pci_driver *driver);

/*
 * Initialize the PCI bus driver
 */
void pci_init(void);

#endif  /* !_PCI_PCI_H_ */
