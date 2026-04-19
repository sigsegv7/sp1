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

#ifndef _MM_PHYSMEM_H_
#define _MM_PHYSMEM_H_ 1

#include <sys/types.h>

/*
 * Initialize the physical memory management
 */
void mm_physmem_init(void);

/*
 * Allocate one or more frames and return the page
 * aligned base
 *
 * @count: Number of frames to allocate
 */
uintptr_t mm_physmem_alloc(size_t count);

/*
 * Free one or more physical memory frames
 *
 * @base:  Memory base to free at
 * @count: Number of frames to free
 */
void mm_physmem_free(uintptr_t base, size_t count);

#endif  /* !_MM_PHYSMEM_H_ */
