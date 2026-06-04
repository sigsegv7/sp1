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

#include <mu/board.h>
#include <machine/hpet.h>
#include <io/clkdev/clkdev.h>
#include <lib/printf.h>

#define pr_trace(fmt, ...) \
    printf("board: " fmt, ##__VA_ARGS__)

status_t
mu_board_init(void)
{
    pr_trace("begin board init\n");
    clkdev_init();
    md_hpet_init();
    return STATUS_SUCCESS;
}
