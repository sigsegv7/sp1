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

#ifndef _COMMON_PTRBOX_H_
#define _COMMON_PTRBOX_H_ 1

/*
 * Represents a pointer box entry in which allocated data can
 * be stored within
 *
 * @data: Allocated data backed by entry
 * @next: Next pointer box entry
 */
struct ptrbox_entry {
    void *data;
    struct ptrbox_entry *next;
};

/*
 * Represents a pointer box that enables RAII like behavior
 *
 * @head: Pointer box head
 * @tail: Pointer box tail
 */
struct ptrbox {
    struct ptrbox_entry *head;
    struct ptrbox_entry *tail;
};

/*
 * Initialize a pointer box
 *
 * @ptrbox: Pointer box to initialize
 *
 * Returns zero on success
 */
int ptrbox_init(struct ptrbox *ptrbox);

/*
 * Allocate memory and save the reference within a pointer box
 *
 * @ptrbox: Pointer box to save reference within
 * @len:    Length of memory to allocate
 *
 * Returns the base of the allocated memory on success, otherwise
 * NULL.
 */
void *ptrbox_alloc(struct ptrbox *ptrbox, size_t len);

/*
 * Duplicate a string and store the reference within a
 * pointer box
 *
 * @ptrbox: Pointer box to store reference within
 * @s:      String to duplicate
 *
 * Returns the duplicated string on success, otherwise NULL.
 */
char *ptrbox_strdup(struct ptrbox *ptrbox, const char *s);

/*
 * Destroy a pointer box along with all references stored
 * within
 *
 * @ptrbox: Pointer box to destroy
 */
void ptrbox_destroy(struct ptrbox *ptrbox);

#endif  /* !_COMMON_PTRBOX_H_ */
