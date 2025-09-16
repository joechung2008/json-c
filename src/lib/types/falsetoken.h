#pragma once

#include <stdbool.h>

struct FalseToken
{
    int  skip;
    int  type;
    bool value;
};

typedef struct FalseToken FalseToken;
