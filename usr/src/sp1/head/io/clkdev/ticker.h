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

#include <sys/units.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <os/cum.h>
#include <os/knot.h>

#define TICKER_NAMESZ 32
#define TICKER_DATA_FROM(OBJ_P) \
    ((struct clk_ticker *)(OBJ_P)->data)

struct clk_ticker;
extern struct cum_object *clkdev_root;

/*
 * Units used by clkdev
 *
 * @CLK_UNIT_NONE:   Units is unset
 * @CLK_UNIT_FS:     Units is femtoseconds
 */
typedef enum {
    CLK_UNIT_NONE,
    CLK_UNIT_FS,
} clk_unit_t;

/*
 * Operations that can be associated with a ticker
 *
 * @get_count: Obtain the current ticker value
 * @set_count: Set the current ticker value
 */
struct ticker_ops {
    size_t(*get_count)(void);
    void(*set_count)(size_t cnt);
};

/*
 * Represents a ticker device i.e., a component that includes a
 * monotonic counter.
 *
 * @name:   Name of ticker device
 * @period: Counter period (a value of 0 indicates unused)
 * @unit:   Units used
 * @ops:    Ticker operations
 */
struct clk_ticker {
    char name[TICKER_NAMESZ];
    size_t period;
    clk_unit_t unit;
    struct ticker_ops ops;
};

/*
 * Convert a number of msecs to a ticker delta
 *
 * @ticker: Ticker to compute from
 * @msec:   Number of miliseconds to offset
 */
__always_inline static inline size_t
ticker_msec_delta(struct clk_ticker *ticker, size_t msec)
{
    if (ticker == NULL) {
        return 0;
    }

    switch (ticker->unit) {
    case CLK_UNIT_FS:
        return (msec * UNIT_FS_PER_MS) / ticker->period;
    default:
        knot("ticker: bad ticker unit in %s()\n", __func__);
        break;
    }
}

/*
 * Obtain the current count from a ticker
 *
 * @ticker: Ticker to obtain count from
 */
__always_inline static inline size_t
ticker_get_count(struct clk_ticker *ticker)
{
    struct ticker_ops *ops;

    if (ticker == NULL) {
        return 0;
    }

    ops = &ticker->ops;
    return (ops->get_count == NULL) ? 0 : ops->get_count();
}

/*
 * Set the current count for a ticker
 *
 * @ticker: Ticker to set for
 * @v:      Value to set
 */
__always_inline static inline void
ticker_set_count(struct clk_ticker *ticker, size_t v)
{
    struct ticker_ops *ops;

    if (ticker == NULL) {
        return;
    }

    ops = &ticker->ops;
    if (ops->set_count != NULL) {
        ops->set_count(v);
    }
}

#endif  /* !_CLKDEV_TICKER_H_ */
