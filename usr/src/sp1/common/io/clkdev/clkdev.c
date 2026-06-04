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

#include <io/clkdev/clkdev.h>
#include <os/cum.h>
#include <os/knot.h>

struct cum_object *clkdev_root;

status_t
clkdev_init(void)
{
    status_t status;

    /* Initialize the clkdev CUM directory */
    status = cum_init_directory("clkdev", &clkdev_root);
    if (status != STATUS_SUCCESS) {
        return status;
    }

    status = cum_directory_add(cum_root, clkdev_root);
    if (status != STATUS_SUCCESS) {
        knot("clkdev: failed to add root\n");
    }

    return STATUS_SUCCESS;
}
