#pragma once

#include "stringtoken.h"
#include "token.h"

struct PairToken
{
    int          skip;
    int          type;
    StringToken *key;
    Token       *value;
};

typedef struct PairToken PairToken;

struct PairTokens
{
    int         size;
    PairToken **pairs;
};

typedef struct PairTokens PairTokens;
