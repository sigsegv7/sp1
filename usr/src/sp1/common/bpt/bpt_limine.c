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
#include <lib/printf.h>

/* HHDM request */
static struct limine_hhdm_response *hhdm_resp = NULL;
static volatile struct limine_hhdm_request hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

/*
 * Obtain the boot protocol variables
 *
 * @res: Result is written here
 */
static status_t
limine_get_protovar(struct bpt_protovar *res)
{
    if (res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    res->kload_base = hhdm_resp->offset;
    return STATUS_SUCCESS;
}

status_t
bpt_init_limine(struct bpt_ops *ops)
{
    if (ops == NULL) {
        return STATUS_INVALID_PARAM;
    }

    hhdm_resp = hhdm_req.response;
    ops->get_protovar = limine_get_protovar;
    return STATUS_SUCCESS;
}
