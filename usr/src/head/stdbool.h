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

#ifndef _STDBOOL_H_ 1
#define _STDBOOL_H_ 1

#ifndef _HAVE_bool
#define _HAVE_bool
typedef _Bool bool;

#ifndef true
#define true 1
#endif  /* !true */

#ifndef false
#define false 0
#endif  /* !false */

#endif  /* !_HAVE_bool */
#endif  /* !_STDBOOL_H_ */
