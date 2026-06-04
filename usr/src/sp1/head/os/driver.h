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

#ifndef _OS_DRIVER_H_
#define _OS_DRIVER_H_ 1

#include <sys/status.h>
#include <sys/types.h>
#include <sys/cdefs.h>

/* Driver attribute */
#define __driver            \
    __attr(used)            \
    __section(".driver")

extern char __driver_start[];
extern char __driver_end[];

/*
 * Represents the driver types
 *
 * @DRIVER_TYPE_NONE: Driver has no associated type
 * @DRIVER_TYPE_PCI:  Driver is a PCI driver
 */
typedef enum {
    DRIVER_TYPE_NONE,
    DRIVER_TYPE_PCI
} driver_type_t;

/*
 * Represents a driver description
 *
 * @name:   Name of driver
 * @author: Name of driver author
 * @init:   Routine to initialize the driver
 */
struct driver_desc {
    const char *name;
    const char *author;
    status_t(*init)(struct driver_desc *desc);
};

/*
 * Initialize all drivers
 */
#define DRIVERS_INIT() \
    for (struct driver_desc *__m = (struct driver_desc *)__driver_start;    \
         (uintptr_t)__m < (uintptr_t)__driver_end; ++__m)         \
    {                                                                   \
        __m->init(__m);                                             \
    }

#endif  /* !_OS_DRIVER_H_ */
