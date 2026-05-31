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

#ifndef _MM_SLAB_H_
#define _MM_SLAB_H_ 1

#include <sys/status.h>
#include <sys/types.h>

/* Maximum entries per magazine */
#define SLAB_MAG_ENTRIES 8

/* Maximum pages per slab */
#define PAGES_PER_SLAB 2

/*
 * Represents a slab of memory that can be allocated from in
 * fixed size chunks depending on the granularity.
 *
 * @pool:   Slab data pool
 * @bitmap: Chunk bitmap
 * @next:   Next slab in the list
 */
struct slab {
    void *pool;
    uint64_t bitmap;
    struct slab *next;
};

/*
 * A slab magazine holds various slabs at different granularities
 *
 * @slab:           Slab list [granularity = 2^(n+5)]
 * @slabs_used:     Number of slabs used
 * @slabs_free:     Number of slabs free
 */
struct slab_mag {
    struct slab slab[SLAB_MAG_ENTRIES];
    size_t slabs_used;
    size_t slabs_free;
};

/*
 * Initialize a slab magazine
 *
 * @mag:  Slab magazine to initialize
 */
status_t slab_mag_init(struct slab_mag *mag);

#endif  /* !_MM_SLAB_H_ */
