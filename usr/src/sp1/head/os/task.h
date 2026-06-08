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

#include <sys/queue.h>
#include <sys/param.h>
#include <sys/status.h>
#include <sys/types.h>
#include <os/schedvar.h>
#include <mu/pcb.h>

/* Task flags */
#define TASK_USER  BIT(0)   /* Is a user task */
#define TASK_RUN   BIT(1)   /* Task is running */

/* Stack */
#define BASE_STACK_SIZE 8192

/*
 * Represents a runnable task in a user/kernel context.
 *
 * @pid:    Process ID of task
 * @flags:  Task flags
 * @pcb:    Process control block
 * @stack_base: Task stack base
 * @stack_size: Task stack size
 * @runq_link:  Queue link
 */
struct task {
    pid_t pid;
    uint32_t flags;
    struct pcb pcb;
    uintptr_t stack_base;
    size_t stack_size;
    TAILQ_ENTRY(task) runq_link;
};

/*
 * Initialize a task structure
 *
 * @flags: Flags for task
 * @ip:    Instruction pointer
 * @res:   Result is written here
 */
status_t task_init(uint32_t flags, uintptr_t ip, struct task *res);

#endif  /* !_OS_TASK_H_ */
