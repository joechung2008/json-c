#pragma once
#include <stddef.h>
#include <stdint.h>
#include "pairtoken.h"

struct ObjectToken
{
    int32_t     skip;
    int32_t     type;
    PairTokens *members;
};

typedef struct ObjectToken ObjectToken;

_Static_assert(offsetof(struct ObjectToken, skip) == 0, "ObjectToken.skip expected at offset 0");
_Static_assert(offsetof(struct ObjectToken, type) == sizeof(int32_t),
               "ObjectToken.type expected immediately after skip");
_Static_assert(offsetof(struct ObjectToken, members) >= 2 * sizeof(int32_t),
               "ObjectToken.members must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct ObjectToken, members) % _Alignof(void *)) == 0,
               "ObjectToken.members pointer must be pointer-aligned");
