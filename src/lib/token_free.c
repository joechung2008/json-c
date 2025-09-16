#include <stdint.h>
#include <stdlib.h>

#include "types.h"
#include "token_free.h"
#include "token_helpers.h"

void token_free(void *tok)
{
    if (!tok)
        return;

    /* Centralized helper for token header access */
    int type = token_get_type((void *)tok);

    switch (type)
    {
    case TOKEN_STRING:
    {
        StringToken *s = (StringToken *)tok;
        free(s->value);
        free(s);
        break;
    }
    case TOKEN_NUMBER:
    {
        NumberToken *n = (NumberToken *)tok;
        free(n);
        break;
    }
    case TOKEN_TRUE:
    case TOKEN_FALSE:
    {
        free(tok);
        break;
    }
    case TOKEN_NULL:
    {
        free(tok);
        break;
    }
    case TOKEN_PAIR:
    {
        PairToken *p = (PairToken *)tok;
        if (p->key)
            token_free(p->key);
        if (p->value)
            token_free(p->value);
        free(p);
        break;
    }
    case TOKEN_ARRAY:
    {
        ArrayToken *a = (ArrayToken *)tok;
        if (a->elements)
        {
            for (int32_t i = 0; i < a->elements->size; ++i)
            {
                token_free(a->elements->tokens[i]);
            }
            free(a->elements->tokens);
            free(a->elements);
        }
        free(a);
        break;
    }
    case TOKEN_OBJECT:
    {
        ObjectToken *o = (ObjectToken *)tok;
        if (o->members)
        {
            for (int32_t i = 0; i < o->members->size; ++i)
            {
                token_free(o->members->pairs[i]);
            }
            free(o->members->pairs);
            free(o->members);
        }
        free(o);
        break;
    }
    default:
        free(tok);
        break;
    }
}

void free_tokens(struct Tokens *t)
{
    if (!t)
        return;
    for (int32_t i = 0; i < t->size; ++i)
    {
        token_free(t->tokens[i]);
    }
    free(t->tokens);
    free(t);
}

void free_pairtokens(struct PairTokens *p)
{
    if (!p)
        return;
    for (int32_t i = 0; i < p->size; ++i)
    {
        token_free(p->pairs[i]);
    }
    free(p->pairs);
    free(p);
}
