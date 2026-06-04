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

/*
 * Represents a PCI device advertisement
 *
 * @vendor_id:  Device vendor ID
 * @device_id:  Device ID
 * @ven_dev:    If set, using vendor/device pair
 */
struct pci_adv {
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t ven_dev : 1;
};

/*
 * Represents a PCI driver
 *
 * @kick: Initializes the hardware associated with the PCI device driver
 */
struct pci_driver {
    status_t(*kick)(struct pci_device *dev);
};

/*
 * Initialize the PCI bus driver
 */
void pci_init(void);

#endif  /* !_PCI_PCI_H_ */
