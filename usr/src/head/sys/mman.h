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

#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_ 1

#include <sys/param.h>

/* Protection flags */
#define PROT_READ   0x00        /* Mapping is readable */
#define PROT_WRITE  BIT(0)      /* Mapping is writable */
#define PROT_EXEC   BIT(1)      /* Mapping is executable */
#define PROT_USER   BIT(2)      /* Mapping is user accessible */

#endif  /* !_SYS_MMAN_H_ */
