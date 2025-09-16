#pragma once

#include "pairtoken.h"

struct ObjectToken
{
    int         skip;
    int         type;
    PairTokens *members;
};

typedef struct ObjectToken ObjectToken;
