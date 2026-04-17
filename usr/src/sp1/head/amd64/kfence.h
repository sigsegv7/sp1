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

#ifndef _MACHINE_KFENCE_H_
#define _MACHINE_KFENCE_H_ 1

/*
 * Kernel fence for interrupt entries that do
 * not have an error code
 */
#define KFENCE              \
    testq $0x3, 8(%rsp)   ; \
    jz 1f                 ; \
    lfence                ; \
    swapgs                ; \
1:  nop

/*
 * Kernel fence for interrupt entries that
 * have an error code
 */
#define KFENCE_EC           \
    testq $0x3, 16(%rsp)  ; \
    jz 1f                 ; \
    lfence                ; \
    swapgs                ; \
1:  nop

#endif  /* !_MACHINE_KFENCE_H_ *
