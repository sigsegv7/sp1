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

#include <string.h>

int
strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2++) {
        if (*s1++ == 0) {
            return (0);
        }
    }

    return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}
