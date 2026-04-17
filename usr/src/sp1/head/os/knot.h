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

#ifndef _OS_KNOT_H_
#define _OS_KNOT_H_ 1

#include <sys/cdefs.h>
#include <stdarg.h>

/*
 * This routine knots and fucks the kernel so it doesn't
 * go anywhere and is halted. This is used during serious
 * errors that may occur.
 *
 * @fmt:   Format specifier
 * @<...>: Vargs
 */
__no_return void knot(const char *fmt, ...);

/*
 * This routine is the main panic handler and should NOT be
 * called directly.
 *
 * @fmt:   Format specifier
 * @<...>: Vargs
 */
__no_return void __knot(const char *fmt, ...);

#endif  /* !_OS_KNOT_H_ */
