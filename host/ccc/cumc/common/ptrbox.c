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

#include <stddef.h>
#include <stdint.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "cumc/ptrbox.h"

/*
 * Allocate a pointer box entry to be used
 */
static struct ptrbox_entry *
ptrbox_alloc_entry(void)
{
    struct ptrbox_entry *entry;

    if ((entry = malloc(sizeof(*entry))) == NULL) {
        return NULL;
    }

    entry->data = NULL;
    entry->next = NULL;
    return entry;
}

static int
ptrbox_push_entry(struct ptrbox *ptrbox, struct ptrbox_entry *entry)
{
    if (ptrbox == NULL || entry == NULL) {
        errno = EINVAL;
        return -1;
    }

    if (ptrbox->head == NULL || ptrbox->tail == NULL) {
        ptrbox->head = entry;
        ptrbox->tail = entry;
    } else {
        ptrbox->head->next = entry;
        ptrbox->head = entry;
    }

    return 0;
}

int
ptrbox_init(struct ptrbox *ptrbox)
{
    if (ptrbox == NULL) {
        errno = EINVAL;
        return -1;
    }

    ptrbox->head = NULL;
    ptrbox->tail = NULL;
    return 0;
}

void *
ptrbox_alloc(struct ptrbox *ptrbox, size_t len)
{
    struct ptrbox_entry *entry;
    void *p;

    if (ptrbox == NULL || len == 0) {
        errno = EINVAL;
        return NULL;
    }

    if ((p = malloc(len)) == NULL) {
        return NULL;
    }

    if ((entry = ptrbox_alloc_entry()) == NULL) {
        free(p);
        return NULL;
    }

    entry->data = p;
    if (ptrbox_push_entry(ptrbox, entry) < 0) {
        free(p);
        free(entry);
        return NULL;
    }

    return p;
}

char *
ptrbox_strdup(struct ptrbox *ptrbox, const char *s)
{
    char *sdup;
    size_t slen;

    if (ptrbox == NULL || s == NULL) {
        errno = EINVAL;
        return NULL;
    }

    slen = strlen(s) + 1;
    if ((sdup = ptrbox_alloc(ptrbox, slen)) == NULL) {
        return NULL;
    }

    memcpy(sdup, s, slen);
    return sdup;
}

void
ptrbox_destroy(struct ptrbox *ptrbox)
{
    struct ptrbox_entry *entry, *tmp;

    if (ptrbox == NULL) {
        return;
    }

    if ((entry = ptrbox->tail) == NULL) {
        return;
    }

    while (entry != NULL) {
        tmp = entry;
        entry = entry->next;

        free(tmp->data);
        free(tmp);
    }
}
