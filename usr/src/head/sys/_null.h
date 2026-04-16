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

#ifndef _SYS__NULL_H_
#define _SYS__NULL_H_ 1

#if !defined(__cplusplus)
#define NULL  ((void *)0)
#else
#if __cplusplus >= 201103L
#define NULL  nullptr
#else
#define NULL ((void *)0)
#endif  /* __cplusplus >= 201103L */
#endif  /* __cplusplus */

#endif  /* !_SYS_NULL_H_ */
