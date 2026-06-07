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

#ifndef _MU_TASK_H_
#define _MU_TASK_H_ 1

#include <sys/types.h>
#include <sys/cdefs.h>
#include <mu/frame.h>

/*
 * Perform a low-level task switch
 *
 * @framep: Trapframe pointer
 */
void mu_task_switch(struct trapframe *framep);

/*
 * Enter tasking mode
 */
__no_return void mu_task_mode(void);

#endif  /* !_MU_TASK_H_ */
