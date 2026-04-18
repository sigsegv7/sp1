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

#include <os/bpt.h>
#include <lib/limine.h>
#include <string.h>

#ifndef _BOOT_PROTO
#error "Boot protocol not defined!"
#else
#define BOOT_PROTO _BOOT_PROTO
#endif  /* !_BOOT_PROTO */

/* Boot protocol translation operations */
static struct bpt_ops ops;

status_t
bpt_get_protovar(struct bpt_protovar *res)
{
    if (res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (ops.get_protovar == NULL) {
        return STATUS_IO_ERROR;
    }

    return ops.get_protovar(res);
}

status_t
bpt_mem_entry_i(size_t index, struct mem_entry *res)
{
    if (res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (ops.mem_entry_i == NULL) {
        return STATUS_IO_ERROR;
    }

    return ops.mem_entry_i(index, res);
}

status_t
bpt_init(void)
{
    switch (*BOOT_PROTO) {
    case 'l':
        if (memcmp(BOOT_PROTO, "limine", 6) == 0) {
            bpt_init_limine(&ops);
            return STATUS_SUCCESS;
        }

        break;
    }

    return STATUS_INVALID_PARAM;
}
