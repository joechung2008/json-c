#pragma once

#include <stddef.h>
#include <stdint.h>

struct NumberToken
{
    int32_t skip;
    int32_t type;
    double  value;
};

typedef struct NumberToken NumberToken;

_Static_assert(offsetof(struct NumberToken, skip) == 0, "NumberToken.skip expected at offset 0");
_Static_assert(offsetof(struct NumberToken, type) == sizeof(int32_t),
               "NumberToken.type expected immediately after skip");
_Static_assert(offsetof(struct NumberToken, value) >= 2 * sizeof(int32_t),
               "NumberToken.value must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct NumberToken, value) % _Alignof(double)) == 0,
               "NumberToken.value must be aligned for double");
