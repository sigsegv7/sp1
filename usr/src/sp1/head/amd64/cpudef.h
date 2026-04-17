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

#ifndef _MACHINE_CPUDEF_H_
#define _MACHINE_CPUDEF_H_ 1

#include <sys/cdefs.h>

#define md_cpu_halt()  \
    __asmv("hlt" ::: "memory")

#define md_cpu_wait() \
    __asmv("pause" ::: "memory")

#define md_cpu_intoff() \
    __asmv("cli" ::: "memory")

#define md_cpu_inton()  \
    __asmv("sti" ::: "memory")

#endif  /* !_MACHINE_CPUDEF_H_ */
