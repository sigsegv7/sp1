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
#include <io/pci/pci.h>
#include <stdbool.h>

status_t
pci_driver_register(struct pci_driver *driver)
{
    struct pci_adv *adv;
    struct pci_device *dev;
    bool found = false;

    if (driver == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if ((adv = driver->adv) == NULL) {
        return STATUS_IO_ERROR;
    }

    /* Call the kick routine per every present device */
    for (size_t i = 0; i < driver->adv_entries; ++i) {
        dev = pci_resolve_adv(adv);
        if (dev != NULL) {
            driver->kick(dev);
            found = true;
        }
    }

    return found ? STATUS_SUCCESS : STATUS_NO_DEV;
}
