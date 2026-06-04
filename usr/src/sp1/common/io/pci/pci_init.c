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
#include <lib/printf.h>
#include <os/knot.h>
#include <mu/pci.h>

#define pr_trace(fmt, ...) \
    printf("pci: " fmt, ##__VA_ARGS__)

/* Globals */
static struct pci_cam_hook cam;

void
pci_init(void)
{
    status_t status;

    status = pci_init_cam(&cam);
    if (status != STATUS_SUCCESS) {
        knot("pci: failed to initialize cam\n");
    }

    pr_trace("initialized cam\n");
}
