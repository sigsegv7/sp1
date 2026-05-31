/*
 * Copyright (c) 2026, Mirocom Laboratories
 * All rights reserved.
 *
 * The following sources are CONFIDENTIAL and PROPRIETARY
 * property of Mirocom Laboratories. Unauthorized copying,
 * use, distrubution or modification of this file, in whole
 * and in part, is strictly prohibited without the prior written
 * consent from Mirocom Laboratories.
 */

#ifndef _MACHINE_HPETREG_H_
#define _MACHINE_HPETREG_H_ 1

#include <sys/types.h>
#include <sys/param.h>

/* HPET registers */
#define HPET_GENERAL_CAP  0x000     /* General Capabilities */
#define HPET_GENERAL_CONF 0x010     /* General config */
#define HPET_GENERAL_INT  0x020     /* General interrupt */
#define HPET_MAIN_CNT     0x0F0     /* Main counter */

/* Constants */
#define HPET_MAX_CLK_PERIOD 0x05F5E100

/* General config bits */
#define HPET_GCONF_EN       BIT(0)  /* Overall enable */
#define HPET_GCONF_LEG_RT   BIT(1)  /* Legacy replacement route */

#endif  /* !_MACHINE_HPETREG_H_ */
