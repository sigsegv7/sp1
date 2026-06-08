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

#include <sys/cdefs.h>
#include <mu/task.h>
#include <mu/cpu.h>
#include <mu/mmu.h>
#include <os/task.h>
#include <os/sched.h>
#include <os/knot.h>
#include <lib/printf.h>
#include <machine/lapic.h>
#include <machine/gdt.h>
#include <string.h>

/*
 * Startup a user thread.
 *
 * @td: Thread to start up.
 */
void
mu_task_kick(struct task *task)
{
    struct trapframe *tfp;
    struct cpu_info *ci;
    uint16_t ds = GDT_UDATA | 3;

    tfp = &task->pcb.tf;
    ci = mu_this_cpu();
    ci->cur_task = task;

    __asmv(
        "mov %0, %%rax\n"
        "push %1\n"
        "push %2\n"
        "push %3\n"
        "push %%rax\n"
        "push %4\n"
        "test $3, %%ax\n"
        "jz 1f\n"
        "lfence\n"
        "1:\n"
        "   iretq"
        :
        : "r" (tfp->cs),
          "r" (ds),
          "r" (tfp->rsp),
          "m" (tfp->rflags),
          "r" (tfp->rip)
    );

    __builtin_unreachable();
}

static void
task_switch(struct trapframe *tf, struct task *task)
{
    struct pcb *pcb;

    if (tf == NULL || task == NULL) {
        knot("bad parameters in %s()\n", __func__);
    }

    pcb = &task->pcb;
    memcpy(tf, &pcb->tf, sizeof(*tf));
    mu_mmu_writevfr(&pcb->vfr);
}

/*
 * TODO: Support kernel segments
 */
void
mu_task_init(struct task *task, uintptr_t ip)
{
    struct pcb *pcb;
    struct trapframe *tfp;

    if (task == NULL) {
        return;
    }

    pcb = &task->pcb;
    tfp = &pcb->tf;
    memset(tfp, 0, sizeof(*tfp));

    tfp->rip = ip;

    /* TODO */
    tfp->cs = GDT_UCODE | 3;
    tfp->ss = GDT_UDATA | 3;

    tfp->rflags = 0x202;
    tfp->rsp = (uintptr_t)task->stack_base + (BASE_STACK_SIZE);
}

void
mu_task_set_ip(struct task *task, uintptr_t ip)
{
    if (task == NULL) {
        return;
    }

    task->pcb.tf.rip = ip;
}

void
mu_task_switch(struct trapframe *framep)
{
    struct task *task;
    struct cpu_info *ci;

    if (__unlikely(framep == NULL)) {
        knot("task switch failure - framep == NULL\n");
    }

    ci = mu_this_cpu();

    /* Grab the next task and enqueue this one */
    task = sched_task_dequeue(&ci->runq);
    if (ci->cur_task != NULL && task != NULL) {
        sched_task_enqueue(&ci->runq, ci->cur_task);
    }

    /* Do we have a new task? */
    if (task == NULL)
        return;

    ci->cur_task = task;
    task_switch(framep, task);
}

__no_return void
mu_task_mode(struct task *root)
{
    __asmv("sti");
    lapic_timer_oneshot_us(9000);

    if (root != NULL) {
        mu_task_kick(root);
    }

    for (;;) {
        __asmv("rep; nop");
    }
}
