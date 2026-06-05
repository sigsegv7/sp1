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
#include <os/bpt.h>
#include <lib/limine.h>
#include <lib/printf.h>
#include <string.h>

/* Memory map request */
static struct limine_memmap_response *memmap_resp = NULL;
static volatile struct limine_memmap_request memmap_req = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

/* HHDM request */
static struct limine_hhdm_response *hhdm_resp = NULL;
static volatile struct limine_hhdm_request hhdm_req = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
};

/* ACPI RSDP request */
static struct limine_rsdp_response *rsdp_resp = NULL;
static volatile struct limine_rsdp_request rsdp_req = {
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

/* Module request */
static struct limine_module_response *mod_resp = NULL;
static volatile struct limine_module_request mod_req = {
    .id = LIMINE_MODULE_REQUEST,
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
    res->rsdp = rsdp_resp->address;
    return STATUS_SUCCESS;
}

/*
 * Obtain a memory map entry by index
 *
 * @index: Index of memory map entry to obtain
 * @res:   Result is written here
 */
static status_t
limine_mem_entry_i(size_t index, struct mem_entry *res)
{
    struct limine_memmap_entry *entry;

    if (res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    /* Don't overflow */
    if (index >= memmap_resp->entry_count) {
        return STATUS_NOT_FOUND;
    }

    /* 1:1 */
    entry = memmap_resp->entries[index];
    res->base = entry->base;
    res->length = entry->length;
    res->type = entry->type;
    return STATUS_SUCCESS;
}

static status_t
limine_module_lookup(const char *path, struct bpt_module *res)
{
    struct limine_file *mod;

    if (path == NULL || res == NULL) {
        return STATUS_INVALID_PARAM;
    }

    if (mod_resp == NULL) {
        return STATUS_NOT_FOUND;
    }

    for (size_t i = 0; i < mod_resp->module_count; ++i) {
        mod = mod_resp->modules[i];
        if (__likely(*mod->path != *path)) {
            continue;
        }

        if (strcmp(mod->path, path) == 0) {
            res->data = mod->address;
            res->len = mod->size;
            return STATUS_SUCCESS;
        }
    }

    return STATUS_NOT_FOUND;
}

status_t
bpt_init_limine(struct bpt_ops *ops)
{
    if (ops == NULL) {
        return STATUS_INVALID_PARAM;
    }

    hhdm_resp = hhdm_req.response;
    memmap_resp = memmap_req.response;
    rsdp_resp = rsdp_req.response;
    mod_resp = mod_req.response;

    ops->get_protovar = limine_get_protovar;
    ops->mem_entry_i = limine_mem_entry_i;
    ops->module_lookup = limine_module_lookup;
    return STATUS_SUCCESS;
}
