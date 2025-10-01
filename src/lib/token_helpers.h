/* Centralized helpers for accessing token header fields.
 *
 * Historically codebase cast arbitrary Token* pointers to concrete
 * token types or used pointer arithmetic to read fields like 'skip' and
 * 'type'. That was fragile and scattered. This header centralizes the
 * assumptions about token layout and provides small helpers.
 */
#pragma once
#include "types/token.h"

/* We expect every concrete token struct to begin with two ints:
 *   int skip;
 *   int type;
 *
 * Provide helpers to access those fields in a single place.
 */
static inline int token_get_skip(void *tok)
{
    if (!tok)
        return 0;
    return (int)(*((int32_t *)tok));
}

static inline int token_get_type(void *tok)
{
    if (!tok)
        return -1;
    /* token 'type' is located after the first int32_t skip field */
    return (int)(*((int32_t *)(((char *)tok) + sizeof(int32_t))));
}
