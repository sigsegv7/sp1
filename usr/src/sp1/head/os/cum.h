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

#ifndef _OS_CUM_H_
#define _OS_CUM_H_ 1

#include <sys/types.h>
#include <sys/status.h>

/* Maximum name length of a CUM object */
#define CUM_NAME_MAXLEN 255

/*
 * Valid CUM object / domain types
 *
 * @CUM_OBJECT_NONE:        This object has no associated type
 * @CUM_OBJECT_DIRECTORY:   This object is a CUM directory
 * @CUM_OBJECT_TIMER:       This object represents a timer
 */
typedef enum {
    CUM_OBJECT_NONE,
    CUM_OBJECT_DIRECTORY,
    CUM_OBJECT_TIMER
} cum_domain_t;

/*
 * Represents a Component Unified-Access Management
 * object.
 *
 * @name: Name of CUM object
 * @data: Underlying data
 * @dom:  CUM domain
 * @next: Next CUM object
 */
struct cum_object {
    char *name;
    void *data;
    cum_domain_t dom;
    struct cum_object *next;
};

/*
 * Allocate a CUM object
 *
 * @name: Name of CUM object to allocate
 * @data: Data to be associated with object [NULLable]
 * @dom:  Domain for object to exist within
 * @res:  Result is written here
 */
status_t cum_init_object(
    char *name, void *data,
    cum_domain_t dom, struct cum_object **res
);

#endif  /* !_OS_CUM_H_ */
