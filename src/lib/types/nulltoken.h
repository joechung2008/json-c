#pragma once
#include <stddef.h>
#include <stdint.h>

struct NullToken
{
    int32_t skip;
    int32_t type;
};

typedef struct NullToken NullToken;

_Static_assert(offsetof(struct NullToken, skip) == 0, "NullToken.skip expected at offset 0");
_Static_assert(offsetof(struct NullToken, type) == sizeof(int32_t), "NullToken.type expected immediately after skip");
