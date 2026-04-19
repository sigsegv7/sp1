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

#ifndef _MACHINE_VFR_H_
#define _MACHINE_VFR_H_ 1

#include <sys/types.h>

/*
 * Virtual fuck region for AMD64
 */
struct mmu_vfr {
    uint64_t cr3;
};

#endif  /* !_MACHINE_VFR_H_ */
