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

void *
memcpy(void *dest, const void *src, size_t count)
{
    if (dest == NULL || src == NULL) {
        return NULL;
    }

    for (size_t i = 0; i < count; ++i) {
        ((char *)dest)[i] = ((char *)src)[i];
    }

    return dest;
}
