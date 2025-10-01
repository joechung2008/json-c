#pragma once
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct TrueToken
{
    int32_t skip;
    int32_t type;
    bool    value;
};

typedef struct TrueToken TrueToken;

_Static_assert(offsetof(struct TrueToken, skip) == 0, "TrueToken.skip expected at offset 0");
_Static_assert(offsetof(struct TrueToken, type) == sizeof(int32_t), "TrueToken.type expected immediately after skip");
_Static_assert(offsetof(struct TrueToken, value) >= 2 * sizeof(int32_t),
               "TrueToken.value must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct TrueToken, value) % _Alignof(bool)) == 0, "TrueToken.value must be aligned for bool");
