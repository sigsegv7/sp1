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

#ifndef _STRING_H_
#define _STRING_H_ 1

#include <sys/types.h>
#include <stddef.h>

/*
 * Fill a buffer with 'n' bytes of 'c'
 */
void *memset(void *s, int c, size_t n);

/*
 * Compare 'n' bytes of two buffers 's1' and 's2'
 */
int memcmp(const void *s1, const void *s2, size_t n);

/*
 * Copy 'count' bytes of 'src' to 'dest'
 */
void *memcpy(void *dest, const void *src, size_t count);

#endif  /* _STRING_H_ */
