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

#ifndef _MACHINE_TLB_H_
#define _MACHINE_TLB_H_ 1

#include <sys/cdefs.h>

/*
 * Flush a single entry from the translation lookaside buffer
 * to force it to be translated again.
 */
__always_inline static void
md_tlb_flush(uintptr_t va)
{
    __asmv(
        "invlpg (%0)"
        :
        : "r" (va)
        : "memory"
    );
}

#endif  /* !_MACHINE_TLB_H_ */
