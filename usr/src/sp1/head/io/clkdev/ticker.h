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

#ifndef _CLKDEV_TICKER_H_
#define _CLKDEV_TICKER_H_ 1

#include <sys/types.h>
#include <os/cum.h>

#define TICKER_NAMESZ 32
#define TICKER_DATA_FROM(OBJ_P) \
    ((struct clk_ticker *)(OBJ_P)->data)

extern struct cum_object *clkdev_root;

/*
 * Represents a ticker device i.e., a component that includes a
 * monotonic counter.
 *
 *
 * @name:   Name of ticker device
 * @period: Counter period (a value of 0 indicates unused)
 */
struct clk_ticker {
    char name[TICKER_NAMESZ];
    size_t period;
};

#endif  /* !_CLKDEV_TICKER_H_ */
