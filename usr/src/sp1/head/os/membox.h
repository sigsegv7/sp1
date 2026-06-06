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

#ifndef _OS_MEMBOX_H_
#define _OS_MEMBOX_H_ 1

#include <sys/status.h>
#include <sys/types.h>

/*
 * Describes the underlying memory kind
 *
 * @MEM_TYPE_NONE:      This type is invalid
 * @MEM_TYPE_PHYSICAL:  Physical memory
 */
typedef enum {
    MEM_TYPE_NONE,
    MEM_TYPE_PHYSICAL,
} membox_type_t;

/*
 * Represents a memory box entry
 *
 * @type:  Memory type
 * @ptr:   Data pointer
 * @size:  Number of bytes in entry
 * @next:  Next entry
 */
struct membox_entry {
    membox_type_t type;
    void *ptr;
    size_t size;
    struct membox_entry *next;
};

/*
 * Represents a memory box
 *
 * @first:  First entry in list
 * @last:   Last entry in list
 */
struct membox {
    struct membox_entry *first;
    struct membox_entry *last;
};

/*
 * Allocate a number of bytes from a desired memory source
 *
 * @membox: Memory box to add to
 * @size: Number of bytes to allocate
 * @type: Memory type
 *
 * Returns the VMA base, only page aligned if PHYSICAL
 */
void *membox_alloc(struct membox *membox, size_t size, membox_type_t type);

/*
 * Destroy all entries associated with a memory box
 *
 * @membox: Memory box to destroy
 */
void membox_destroy(struct membox *membox);

/*
 * Initialize a memory box
 *
 * @membox: Memory box to initialize
 */
status_t membox_init(struct membox *membox);

#endif  /* !_OS_MEMBOX_H_ */
