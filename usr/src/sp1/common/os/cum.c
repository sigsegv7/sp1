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

#include <sys/limits.h>
#include <sys/status.h>
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

status_t
cum_resolve(struct cum_resolve *rp)
{
    status_t status = STATUS_SUCCESS;
    struct cum_object *obj = cum_root;
    char namebuf[NAMESZ];
    uint16_t namebuf_idx = 0;
    const char *p;
    char tmp;

    if (rp == NULL) {
        return STATUS_INVALID_PARAM;
    }

    p = rp->path;

    /* Strip leading slashes */
    while (*p == '/')
        ++p;

    while (*p != '\0') {
        if (*p == '/' || p[1] == '\0') {
            tmp = *p++;
            namebuf[namebuf_idx] = (tmp == '/') ? '\0' : tmp;
            namebuf[namebuf_idx + 1] = '\0';
            namebuf_idx = 0;
            status = cum_lookup_single(obj, namebuf, &obj);
            continue;
        }

        if (status != STATUS_SUCCESS) {
            return status;
        }

        namebuf[namebuf_idx++] = *p++;
        if (namebuf_idx >= NAMESZ - 2) {
            return STATUS_NAME_TOO_LONG;
        }
    }

    rp->lookup = obj;
    return STATUS_SUCCESS;
}
