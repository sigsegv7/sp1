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

#ifndef _MU_MMIO_H_
#define _MU_MMIO_H_ 1

#include <sys/cdefs.h>

/* Builds mmio_write<n> functions */
#define _MMIO_WRITE_BUILDER(NAME, TYPE)     \
    static inline void                      \
    mmio_##NAME(TYPE *ptr, TYPE val)        \
    {                                       \
        __barrier();                        \
        *(volatile TYPE *)ptr = val;        \
    }

/* Builds mmio_read<n> functions */
#define _MMIO_READ_BUILDER(NAME, TYPE)      \
    static inline TYPE                      \
    mmio_##NAME(TYPE *ptr)                  \
    {                                       \
        __barrier();                        \
        return *(volatile TYPE *)ptr;       \
    }

/* mmio_write<n> */
_MMIO_WRITE_BUILDER(write8, uint8_t);
_MMIO_WRITE_BUILDER(write16, uint16_t);
_MMIO_WRITE_BUILDER(write32, uint32_t);
#if __SIZEOF_SIZE_T__ == 8
_MMIO_WRITE_BUILDER(write64, uint64_t);
#endif

/* Builds mmio_read<n> functions */
_MMIO_READ_BUILDER(read8, uint8_t);
_MMIO_READ_BUILDER(read16, uint16_t);
_MMIO_READ_BUILDER(read32, uint32_t);
#if __SIZEOF_SIZE_T__ == 8
_MMIO_READ_BUILDER(read64, uint64_t);
#endif

#endif  /* !_MU_MMIO_H_ */
