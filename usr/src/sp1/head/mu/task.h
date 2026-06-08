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
#include <os/task.h>
#include <mu/frame.h>

/*
 * Perform a low-level task switch
 *
 * @framep: Trapframe pointer
 */
void mu_task_switch(struct trapframe *framep);

/*
 * Initialize low-level task fields
 *
 * @task: Task to initialize
 * @ip:   Instruction pointer
 */
void mu_task_init(struct task *task, uintptr_t ip);

/*
 * Instruction pointer to set
 *
 * @task: Task to set IP of
 * @ip:   IP to set
 */
void mu_task_set_ip(struct task *task, uintptr_t ip);

/*
 * Start a user task
 */
void mu_task_kick(struct task *task);

/*
 * Enter tasking mode
 *
 * @root: Root task
 */
__no_return void mu_task_mode(struct task *root);

#endif  /* !_MU_TASK_H_ */
