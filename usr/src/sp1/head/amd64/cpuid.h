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

#ifndef _MACHINE_CPUID_H_
#define _MACHINE_CPUID_H_ 1

#include <sys/cdefs.h>

#define __cpuid(level, a, b, c, d)                      \
    __asmv("cpuid\n\t"                                  \
            : "=a" (a), "=b" (b), "=c" (c), "=d" (d)    \
            : "0" (level))

#endif  /* !_MACHINE_CPUID_H_ */
