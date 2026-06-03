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

#include <os/cum.h>

/* Root CUM object */
struct cum_object *cum_root;

status_t
cum_init(void)
{
    status_t status;

    status = cum_init_directory(
        CUM_ROOT_NAME,
        &cum_root
    );

    if (status != STATUS_SUCCESS) {
        return status;
    }

    return STATUS_SUCCESS;
}
