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

#ifndef _FRONTEND_TOKEN_H_
#define _FRONTEND_TOKEN_H_ 1

/*
 * Represents valid program token types
 */
typedef enum {
    TT_NONE,        /* [NONE] */
    TT_PLUS,        /* '+' */
    TT_MINUS,       /* '-' */
    TT_SLASH,       /* '/' */
    TT_STAR,        /* '*' */
} tt_t;

/*
 * Represents a program source token
 *
 * @type: Token type
 */
struct token {
    tt_t type;
    union {
        char c;
    };
};

#endif  /* !_FRONTEND_TOKEN_H_ */
