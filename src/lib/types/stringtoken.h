#pragma once

struct StringToken
{
    int   skip;
    int   type;
    char *value;
};

typedef struct StringToken StringToken;
