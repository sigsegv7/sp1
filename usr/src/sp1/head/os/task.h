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

#ifndef _OS_TASK_H_
#define _OS_TASK_H_ 1

#include <sys/param.h>
#include <sys/status.h>
#include <sys/types.h>
#include <mu/pcb.h>

/* Task flags */
#define TASK_USER  BIT(0)   /* Is a user task */
#define TASK_RUN   BIT(1)   /* Task is running */

/*
 * Represents a runnable task in a user/kernel context.
 *
 * @pid:    Process ID of task
 * @flags:  Task flags
 * @pcb:    Process control block
 */
struct task {
    pid_t pid;
    uint32_t flags;
    struct pcb pcb;
};

/*
 * Initialize a task structure
 *
 * @flags: Flags for task
 * @res:   Result is written here
 */
status_t task_init(uint32_t flags, struct task *res);

#endif  /* !_OS_TASK_H_ */
