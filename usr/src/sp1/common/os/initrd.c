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

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>
#include <os/initrd.h>
#include <os/bpt.h>
#include <os/knot.h>
#include <string.h>

static struct bpt_module initrd_mod;

#define REF_MAGIC "CUMHOLE"
#define REF_MAGIC_LEN 8
#define REF_PATH_LEN 256

/*
 * A reference is an entry within the table that refers
 * to a block of data after the reference table.
 *
 * @magic:      Reference magic
 * @path:       File path
 * @size:       File size
 * @data_off:   Data offset
 */
struct __packed hole_ref {
    char magic[REF_MAGIC_LEN];
    char path[REF_PATH_LEN];
    size_t size;
    size_t data_off;
};

status_t
initrd_lookup(const char *path, struct initrd_file *res)
{
    struct hole_ref *ref;

    if (path == NULL || res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (*path != '/') {
        return STATUS_NOT_FOUND;
    }

    ++path;
    ref = initrd_mod.data;

    for (;;) {
        if (strcmp(ref->magic, REF_MAGIC) != 0) {
            break;
        }

        if (__likely(ref->path[0] != *path)) {
            ++ref;
            continue;
        }

        if (strcmp(ref->path, path) == 0) {
            res->data = PTR_OFFSET(initrd_mod.data, ref->data_off);
            res->sz = ref->size;
            return STATUS_SUCCESS;
        }

        ++ref;
    }

    return STATUS_NOT_FOUND;
}

void
initrd_init(void)
{
    status_t status;

    status = bpt_module_lookup("/boot/initrd.hole", &initrd_mod);
    if (status != STATUS_SUCCESS) {
        knot("initrd: failed to open /boot/initrd.hole\n");
    }
}
