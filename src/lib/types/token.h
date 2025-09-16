#pragma once

typedef void Token;

#include <stddef.h>
#include <stdint.h>

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
    int32_t size;
    Token **tokens;
};

typedef struct Tokens Tokens;

_Static_assert(offsetof(struct Tokens, size) == 0, "Tokens.size expected at offset 0");
/* The 'tokens' pointer must come after the 32-bit size field, but platforms
 * may insert padding to align pointers. Require it's located at or after
 * sizeof(int32_t) and is pointer-aligned instead of requiring an exact offset.
 */
_Static_assert(offsetof(struct Tokens, tokens) >= sizeof(int32_t),
               "Tokens.tokens must come after size (padding allowed)");
_Static_assert((offsetof(struct Tokens, tokens) % _Alignof(void *)) == 0,
               "Tokens.tokens pointer must be pointer-aligned");
