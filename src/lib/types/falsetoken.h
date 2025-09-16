#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct FalseToken
{
    int32_t skip;
    int32_t type;
    bool    value;
};

typedef struct FalseToken FalseToken;

_Static_assert(offsetof(struct FalseToken, skip) == 0, "FalseToken.skip expected at offset 0");
_Static_assert(offsetof(struct FalseToken, type) == sizeof(int32_t), "FalseToken.type expected immediately after skip");
_Static_assert(offsetof(struct FalseToken, value) >= 2 * sizeof(int32_t),
               "FalseToken.value must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct FalseToken, value) % _Alignof(bool)) == 0, "FalseToken.value must be aligned for bool");
