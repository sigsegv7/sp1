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


#ifndef _SYS_ATOMIC_H_
#define _SYS_ATOMIC_H_

#include <sys/types.h>

static inline unsigned long
atomic_add_long_nv(volatile unsigned long *p, unsigned long v)
{
    return __sync_add_and_fetch(p, v);
}

static inline unsigned int
atomic_add_int_nv(volatile unsigned int *p, unsigned int v)
{
    return __sync_add_and_fetch(p, v);
}

static inline unsigned int
atomic_add_64_nv(volatile uint64_t *p, unsigned int v)
{
    return __sync_add_and_fetch(p, v);
}

static inline unsigned long
atomic_sub_long_nv(volatile unsigned long *p, unsigned long v)
{
    return __sync_sub_and_fetch(p, v);
}

static inline unsigned int
atomic_sub_int_nv(volatile unsigned int *p, unsigned int v)
{
    return __sync_sub_and_fetch(p, v);
}

static inline unsigned int
atomic_sub_64_nv(volatile uint64_t *p, unsigned int v)
{
    return __sync_sub_and_fetch(p, v);
}

static inline unsigned int
atomic_load_int_nv(volatile unsigned int *p, unsigned int v)
{
    return __atomic_load_n(p, v);
}

static inline unsigned int
atomic_load_long_nv(volatile unsigned long *p, unsigned int v)
{
    return __atomic_load_n(p, v);
}

static inline unsigned int
atomic_load_64_nv(volatile uint64_t *p, unsigned int v)
{
    return __atomic_load_n(p, v);
}

static inline void
atomic_store_int_nv(volatile unsigned int *p, int nv, unsigned int v)
{
    __atomic_store_n(p, nv, v);
}

static inline void
atomic_store_long_nv(volatile unsigned long *p, long nv, unsigned int v)
{
    __atomic_store_n(p, nv, v);
}

static inline void
atomic_store_64_nv(volatile uint64_t *p, long nv, unsigned int v)
{
    __atomic_store_n(p, nv, v);
}

/* Atomic increment (and fetch) operations */
#define atomic_inc_long(P) atomic_add_long_nv((P), 1)
#define atomic_inc_int(P) atomic_add_int_nv((P), 1)
#define atomic_inc_64(P) atomic_add_64_nv((P), 1)

/* Atomic decrement (and fetch) operations */
#define atomic_dec_long(P) atomic_sub_long_nv((P), 1)
#define atomic_dec_int(P) atomic_sub_int_nv((P), 1)
#define atomic_dec_64(P) atomic_sub_64_nv((P), 1)

/* Atomic load operations */
#define atomic_load_int(P) atomic_load_int_nv((P), __ATOMIC_SEQ_CST)
#define atomic_load_long(P) atomic_load_long_nv((P), __ATOMIC_SEQ_CST)
#define atomic_load_64(P) atomic_load_64_nv((P), __ATOMIC_SEQ_CST)

/* Atomic store operations */
#define atomic_store_int(P, NV) atomic_store_int_nv((P), (NV), __ATOMIC_SEQ_CST)
#define atomic_store_long(P, NV) atomic_store_long_nv((P), (NV), __ATOMIC_SEQ_CST)
#define atomic_store_64(P, NV) atomic_store_64_nv((P), (NV), __ATOMIC_SEQ_CST)

#endif  /* !_SYS_ATOMIC_H_ */
