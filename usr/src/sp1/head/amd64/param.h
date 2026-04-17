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

#ifndef _MACHINE_PARAM_H_
#define _MACHINE_PARAM_H_ 1

/* Presumed cache-line size */
#ifndef COHERENCY_UNIT
#define COHERENCY_UNIT 64
#endif  /* !COHERENCY_UNIT */

/* Pagesize in bytes */
#define PAGESIZE 4096

#endif  /* !_MACHINE_PARAM_H_ */
