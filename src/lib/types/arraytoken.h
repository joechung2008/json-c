#pragma once
#include <stdint.h>
#include <stddef.h>
#include "token.h"

struct ArrayToken
{
    int32_t skip;
    int32_t type;
    Tokens *elements;
};

typedef struct ArrayToken ArrayToken;

_Static_assert(offsetof(struct ArrayToken, skip) == 0, "ArrayToken.skip expected at offset 0");
_Static_assert(offsetof(struct ArrayToken, type) == sizeof(int32_t), "ArrayToken.type expected immediately after skip");
_Static_assert(offsetof(struct ArrayToken, elements) >= 2 * sizeof(int32_t),
               "ArrayToken.elements must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct ArrayToken, elements) % _Alignof(void *)) == 0,
               "ArrayToken.elements pointer must be pointer-aligned");
