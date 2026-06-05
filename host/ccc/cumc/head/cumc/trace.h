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

#ifndef _CUMC_TRACE_H_
#define _CUMC_TRACE_H_ 1

#include <stdio.h>

#define cum_trace(fmt, ...) \
    printf("[\033[32;40mtrace\033[0m]: " fmt, ##__VA_ARGS__)
#define cum_error(fmt, ...) \
    printf("[\033[31;40merror\033[0m]: " fmt, ##__VA_ARGS__)
#define cum_warn(fmt, ...) \
    printf("[\033[35;40mwarn\033[0m]: " fmt, ##__VA_ARGS__)

#endif  /* !_CUMC_TRACE_H_ */
