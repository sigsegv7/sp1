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

status_t
bpt_init_limine(struct bpt_ops *ops)
{
    if (ops == NULL) {
        return STATUS_INVALID_PARAM;
    }

    hhdm_resp = hhdm_req.response;
    memmap_resp = memmap_req.response;
    rsdp_resp = rsdp_req.response;

    ops->get_protovar = limine_get_protovar;
    ops->mem_entry_i = limine_mem_entry_i;
    return STATUS_SUCCESS;
}
