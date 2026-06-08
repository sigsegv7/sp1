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

#ifndef _OS_SCHED_H_
#define _OS_SCHED_H_ 1

#include <os/schedvar.h>

/*
 * Task a task to a scheduler run queue
 *
 * @runq: Run queue to add task to
 * @task: Task to add to run queue
 */
void sched_task_enqueue(struct sched_runq *runq, struct task *task);

/*
 * Deuque a task from a run queue
 *
 * @runq: Run queue to pop from
 *
 * Returns NULL if no more entries
 */
struct task *sched_task_dequeue(struct sched_runq *runq);

#endif  /* !_OS_SCHED_H_ */
