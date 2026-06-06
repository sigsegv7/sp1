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


#ifndef _MACHINE_PRIM_H_
#define _MACHINE_PRIM_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>

/* Hint to the processor that we are in a spinloop */
#define md_cpu_spinwait() \
    __asmv("pause" ::: "memory")

/* Halt the current processor core */
#define md_cpu_halt()  \
    __asmv("hlt" ::: "memory")

/* Disable IRQs */
#define md_cpu_irqoff() \
    __asmv("cli" ::: "memory")

/* Enable IRQs */
#define md_cpu_irqon() \
    __asmv("sti" ::: "memory")

/*
 * Return true if IRQs are enabled
 */
bool md_cpu_irqen(void);

/*
 * Atomic swap operation
 *
 * @p: Location to swap with `v`
 * @v: Value to swap to `p`
 */
__always_inline static inline size_t
md_cpu_aswap(size_t *p, size_t v)
{
    size_t vret;

    if (p == NULL) {
        return 0;
    }

    vret = *(volatile size_t *)p;
    __asmv(
        "xchg %0, %1\n"
        : "+m" (*p), "+r" (v)
        :
        : "memory"
    );

    return vret;
}

#endif  /* !_MACHINE_PRIM_H_ */
