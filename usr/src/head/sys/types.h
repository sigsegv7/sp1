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

#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_ 1

#if defined(_KERNEL) || defined(_SP1_SOURCE)
#include <sys/_null.h>
#endif  /* _KERNEL || _SP1_SOURCE */

/* Unsigned types */
typedef unsigned char       __uint8_t;
typedef unsigned short      __uint16_t;
typedef unsigned int        __uint32_t;
typedef unsigned long long  __uint64_t;
typedef __uint64_t          __size_t;
typedef __uint64_t          __uintptr_t;

/* Signed types */
typedef int         __int8_t;
typedef short       __int16_t;
typedef int         __int32_t;
typedef long long   __int64_t;
typedef __int64_t   __ssize_t;
typedef __ssize_t   __off_t;

#if defined(_KERNEL) || defined(_SP1_SOURCE)
typedef __uint8_t   uint8_t;
typedef __uint16_t  uint16_t;
typedef __uint32_t  uint32_t;
typedef __uint64_t  uint64_t;
typedef __uint64_t  uintptr_t;
typedef __size_t  size_t;

typedef __int8_t    int8_t;
typedef __int16_t   int16_t;
typedef __int32_t   int32_t;
typedef __int64_t   int64_t;

typedef __ssize_t ssize_t;
typedef __ssize_t intmax_t;

typedef ssize_t ptrdiff_t;
#endif  /* !_KERNEL || _SP1_SOURCE */

typedef int pid_t;

#endif  /* !_SYS_TYPES_H_ */
