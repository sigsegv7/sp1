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

#ifndef _SYS_CDEFS_H_
#define _SYS_CDEFS_H_ 1

#define __asmv              __asm__ __volatile__
#define __attr(x)           __attribute__((x))
#define __section(x)        __attr(section((x)))
#define __align(n)          __attr(aligned((n)))
#define __packed            __attr(packed)
#define __no_return         __attr(noreturn)
#define __always_inline     __attr(always_inline)
#define __barrier()         __asmv("" ::: "memory")
#define __likely(exp)   __builtin_expect(((exp) != 0), 1)
#define __unlikely(exp) __builtin_expect(((exp) != 0), 0)

#ifndef offsetof
#define offsetof(st, m) ((size_t)&(((st *)0)->m))
#endif  /* offsetof */

#endif  /* !_SYS_CDEFS_H_ */
