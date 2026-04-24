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

#ifndef _MACHINE_IRQCHIP_H_
#define _MACHINE_IRQCHIP_H_ 1

#include <sys/status.h>

/*
 * Initialize platform interrupt controller chips
 */
status_t md_irqchip_init(void);

#endif  /* !_MACHINE_IRQCHIP_H_ */
