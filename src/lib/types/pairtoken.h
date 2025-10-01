#pragma once
#include <stddef.h>
#include <stdint.h>
#include "stringtoken.h"
#include "token.h"

struct PairToken
{
    int32_t      skip;
    int32_t      type;
    StringToken *key;
    Token       *value;
};

typedef struct PairToken PairToken;

_Static_assert(offsetof(struct PairToken, skip) == 0, "PairToken.skip expected at offset 0");
_Static_assert(offsetof(struct PairToken, type) == sizeof(int32_t), "PairToken.type expected immediately after skip");
_Static_assert(offsetof(struct PairToken, key) >= 2 * sizeof(int32_t),
               "PairToken.key must come after two int32 fields (padding allowed)");
_Static_assert((offsetof(struct PairToken, key) % _Alignof(void *)) == 0,
               "PairToken.key pointer must be pointer-aligned");

_Static_assert(offsetof(struct PairToken, value) >= offsetof(struct PairToken, key) + sizeof(void *),
               "PairToken.value must come after key (padding allowed)");
_Static_assert((offsetof(struct PairToken, value) % _Alignof(void *)) == 0,
               "PairToken.value pointer must be pointer-aligned");

struct PairTokens
{
    int32_t     size;
    PairToken **pairs;
};

typedef struct PairTokens PairTokens;

_Static_assert(offsetof(struct PairTokens, size) == 0, "PairTokens.size expected at offset 0");
_Static_assert(offsetof(struct PairTokens, pairs) >= sizeof(int32_t),
               "PairTokens.pairs must come after size (padding allowed)");
_Static_assert((offsetof(struct PairTokens, pairs) % _Alignof(void *)) == 0,
               "PairTokens.pairs pointer must be pointer-aligned");
