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

#ifndef _PCI_DEVICE_H_
#define _PCI_DEVICE_H_ 1

#include <sys/types.h>

/*
 * Descriptor of device present on PCI bus
 *
 * @function:  PCI function number
 * @device:    PCI device number
 * @bus:       PCI bus number
 */
struct pci_device {
    uint8_t function : 3;
    uint8_t device   : 5;
    uint8_t bus;
};

#endif  /* !_PCI_DEVICE_H_ */
