#pragma once

typedef void Token;

/* Token type tags used at runtime so we can free tokens generically */
enum TokenType
{
    TOKEN_NULL = 0,
    TOKEN_TRUE,
    TOKEN_FALSE,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_ARRAY,
    TOKEN_OBJECT,
    TOKEN_PAIR
};

struct Tokens
{
    int     size;
    Token **tokens;
};

typedef struct Tokens Tokens;
