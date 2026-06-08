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

#ifndef _SYS_SYSCALL_H_
#define _SYS_SYSCALL_H_ 1

#include <sys/types.h>

#define SYS_NONE   0
#define SYS_SYSLOG 1

#if defined(_KERNEL)
/* Syscall return value and arg type */
typedef ssize_t scret_t;
typedef ssize_t scarg_t;

struct syscall_args {
    scarg_t arg0;
    scarg_t arg1;
    scarg_t arg2;
    scarg_t arg3;
    scarg_t arg4;
    scarg_t arg5;
    scarg_t arg6;
    struct trapframe *tf;
};

extern const size_t MAX_SYSCALLS;
extern scret_t(*g_sctab[])(struct syscall_args *);
#endif  /* _KERNEL */
#endif  /* !_SYS_SYSCALL_H_ */
