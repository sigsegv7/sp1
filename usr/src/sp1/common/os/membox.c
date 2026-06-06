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

#include <sys/param.h>
#include <os/membox.h>
#include <mm/kalloc.h>
#include <mm/physmem.h>
#include <mm/vm.h>
#include <mu/param.h>
#include <string.h>

/*
 * Allocate a memory box entry
 *
 * @type: Type of memory box entry to allocate
 */
static struct membox_entry *
membox_alloc_entry(membox_type_t type)
{
    struct membox_entry *entry;

    entry = kalloc(sizeof(*entry));
    if (entry == NULL) {
        return NULL;
    }

    entry->ptr = NULL;
    entry->next = NULL;
    entry->size = 0;
    entry->type = type;
    return entry;
}

/*
 * Append an entry to a memory box
 *
 * @membox: Memory box to append to
 * @entry:  Entry to append
 */
static void
membox_append(struct membox *membox, struct membox_entry *entry)
{
    if (membox == NULL || entry == NULL) {
        return;
    }

    if (membox->first == NULL || membox->last == NULL) {
        membox->first = entry;
        membox->last  = entry;
    } else {
        membox->last->next = entry;
        membox->last = entry;
    }
}

void *
membox_alloc(struct membox *membox, size_t size, membox_type_t type)
{
    struct membox_entry *entry;
    uintptr_t pma;
    void *p;

    entry = membox_alloc_entry(type);
    if (entry == NULL) {
        return NULL;
    }

    switch (type) {
    case MEM_TYPE_PHYSICAL:
        size = ALIGN_UP(size, PAGESIZE);
        pma = mm_physmem_alloc(size / PAGESIZE);
        if (pma == 0) {
            return NULL;
        }

        entry->ptr = pma_to_vma(pma);
        entry->size = size;

        memset(entry->ptr, 0, entry->size);
        membox_append(membox, entry);
        return entry->ptr;
    default:
        kfree(entry);
        return NULL;
    }
}

void
membox_destroy(struct membox *membox)
{
    struct membox_entry *ent;

    if (membox == NULL) {
        return;
    }

    ent = membox->first;
    while (ent != NULL) {
        switch (ent->type) {
        case MEM_TYPE_PHYSICAL:
            mm_physmem_free(vma_to_pma(ent->ptr), ent->size / PAGESIZE);
            break;
        default:
            break;
        }

        ent = ent->next;
    }
}

status_t
membox_init(struct membox *membox)
{
    if (membox == NULL) {
        return STATUS_INVALID_PARAM;
    }

    membox->first = NULL;
    membox->last = NULL;
    return STATUS_SUCCESS;
}
