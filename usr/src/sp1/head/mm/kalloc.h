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

#ifndef _VM_KALLOC_H_
#define _VM_KALLOC_H_

#include <sys/types.h>

/*
 * Allocate a chunk of memory of a desired size
 *
 * @sz: Size of memory to allocate
 *
 * Returns base of memory on success
 */
void *kalloc(size_t sz);

/*
 * Same as kalloc() but allows the specification of an alignent
 * constraint
 */
void *kalloc_memalign(size_t sz, size_t align);

/*
 * Resize an allocated buffer
 *
 * @old_ptr: Pointer to old buffer
 * @newsize: Number of bytes to resize buffer to
 *
 * Returns base of new memory on success
 */
void *krealloc(void *old_ptr, size_t newsize);

/*
 * Deallocate previously allocated memory
 *
 * @ptr: Memory to free
 */
void kfree(void *ptr);

/* USED INTERNALLY */
void kalloc_init(void);

#endif  /* !_VM_KALLOC_H_ */
