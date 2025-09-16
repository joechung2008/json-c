#pragma once

#include "token.h"

struct ArrayToken
{
    int     skip;
    int     type;
    Tokens *elements;
};

typedef struct ArrayToken ArrayToken;
