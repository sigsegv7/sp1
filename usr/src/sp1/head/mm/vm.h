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

#ifndef _MM_VM_H_
#define _MM_VM_H_ 1

#include <sys/param.h>
#include <os/bpt.h>

/*
 * Macros used to convert physical to virtual addresses
 * and vice versa.
 */
#define pma_to_vma(pma) \
    PTR_OFFSET((void *)pma, bpt_kload_base())
#define vma_to_pma(vma) \
    (uintptr_t)PTR_NOFFSET(vma, bpt_kload_base())

#endif  /* !_MM_VM_H_ */
