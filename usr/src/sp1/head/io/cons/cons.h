/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distrubution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#ifndef _CONS_CONS_H_
#define _CONS_CONS_H_ 1

#include <sys/types.h>

/*
 * Represents console attributes
 *
 * @fg: Console foreground
 * @bg: Console background
 */
struct cons_attr {
    uint32_t fg;
    uint32_t bg;
};

/*
 * Initialize the boot console
 *
 * @attr: Console attributes
 */
void cons_init(struct cons_attr *attr);

/*
 * Write to the boot console
 *
 * @str: string to write
 * @len: length of string to write
 */
void cons_write(const char *str, size_t len);

#endif  /* !_CONS_CONS_H_ */
