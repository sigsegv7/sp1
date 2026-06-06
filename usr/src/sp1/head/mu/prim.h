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

#ifndef _MU_PRIM_H_
#define _MU_PRIM_H_ 1

#include <machine/prim.h>   /* per-arch */

#ifndef mu_cpu_spinwait
#define mu_cpu_spinwait() md_cpu_spinwait()
#endif  /* !mu_cpu_spinwait */

#ifndef mu_cpu_mut
#define mu_cpu_mut() md_cpu_mut()
#endif  /* !mu_cpu_mut */

#ifndef mu_cpu_aswap
#define mu_cpu_aswap(ptr, v) md_cpu_aswap((ptr), (v))
#endif  /* !mu_cpu_aswap */

#ifndef mu_cpu_irqoff
#define mu_cpu_irqoff() md_cpu_irqoff
#endif  /* !mu_cpu_irqoff */

#ifndef mu_cpu_irqon
#define mu_cpu_irqon() md_cpu_irqon
#endif  /* !mu_cpu_irqon */

#ifndef mu_cpu_irqen
#define mu_cpu_irqen() md_cpu_irqen
#endif  /* !mu_cpu_irqen */

#endif  /* !_MU_PRIM_H_ */
