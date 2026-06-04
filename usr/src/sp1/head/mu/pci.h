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

#ifndef _MU_PCI_H_
#define _MU_PCI_H_ 1

#include <sys/types.h>
#include <sys/status.h>
#include <io/pci/device.h>

/*
 * PCI configuration access mechanism hooks
 */
struct pci_cam_hook {
    uint32_t(*cam_readl)(struct pci_device *device, uint8_t reg);
    void(*cam_writel)(struct pci_device *device, uint8_t reg, uint32_t v);
};

/*
 * Initialize the PCI CAM hooks
 *
 * @hook_res: Hook result is written here
 */
status_t pci_init_cam(struct pci_cam_hook *hook_res);

#endif  /* !_MU_PCI_H_ */
