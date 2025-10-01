#pragma once
#include <stddef.h>
#include <stdint.h>

struct StringToken
{
    int32_t skip;
    int32_t type;
    char   *value;
};

typedef struct StringToken StringToken;

_Static_assert(offsetof(struct StringToken, skip) == 0, "StringToken.skip expected at offset 0");
_Static_assert(offsetof(struct StringToken, type) == sizeof(int32_t),
               "StringToken.type expected immediately after skip");
_Static_assert(offsetof(struct StringToken, value) >= 2 * sizeof(int32_t),
               "StringToken.value must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct StringToken, value) % _Alignof(void *)) == 0,
               "StringToken.value pointer must be pointer-aligned");
