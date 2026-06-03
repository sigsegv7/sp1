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
#include <sys/status.h>
#include <os/cum.h>
#include <mm/kalloc.h>
#include <string.h>

/*
 * Write a name to a CUM object and perform length checks
 *
 * @name: Name of CUM object to copy
 * @res:  Resulting CUM object to have name copied to
 */
static status_t
cum_write_name(const char *name, struct cum_object *res)
{
    size_t slen;
    char *new_p;

    if (res == NULL || name == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (strlen(name) >= CUM_NAME_MAXLEN - 1) {
        return STATUS_NAME_TOO_LONG;
    }

    slen = strlen(name);
    if ((new_p = kalloc(slen + 1)) == NULL) {
        return STATUS_NO_MEMORY;
    }

    memcpy(new_p, name, slen + 1);
    res->name = new_p;
    return STATUS_SUCCESS;
}

status_t
cum_init_object(char *name, void *data, cum_domain_t dom, struct cum_object **res)
{
    struct cum_object *cum;
    status_t status;

    if (name == NULL || res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if ((cum = kalloc(sizeof(*cum))) == NULL) {
        return STATUS_NO_MEMORY;
    }

    status = cum_write_name(name, cum);
    if (status != STATUS_SUCCESS) {
        kfree(cum);
        return status;
    }

    cum->dom = dom;
    cum->data = data;
    cum->next = NULL;
    *res = cum;
    return STATUS_SUCCESS;
}
