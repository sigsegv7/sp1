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

#ifndef _LIB_ASSERT_H_
#define _LIB_ASSERT_H_ 1

#define __assert(condition)                                         \
    if ((uintptr_t)(condition) == 0) {                              \
        knot("assert \"%s\" failed (%s() at %s:%d)\n", #condition,  \
              __func__, __FILE__, __LINE__);                        \
    }

#endif  /* !_LIB_ASSERT_H_ */
