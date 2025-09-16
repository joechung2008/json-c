#include <string.h>
#include <stdlib.h>

#include "array.h"
#include "shared.h"
#include "value.h"
#include "../token_free.h"
#include "../token_helpers.h"

// Token header contract: every token begins with two ints (skip, type).
// Use TOKEN_SKIP(t) and TOKEN_TYPE(t) from src/lib/token_helpers.h to access these fields.
ArrayToken *json_parse_array(char *s)
{
    enum
    {
        Scanning,
        Elements,
        Comma,
        End
    };

    int     mode     = Scanning;
    int     pos      = 0;
    Tokens *elements = NULL;

    while (pos < strlen(s) && mode != End)
    {
        char ch = s[pos];

        switch (mode)
        {
        case Scanning:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            {
                pos++;
            }
            else if (ch == '[')
            {
                pos++;
                mode = Elements;
            }
            else
            {
                goto fail;
            }
            break;

        case Elements:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            {
                pos++;
            }
            else if (ch == ']')
            {
                /* If we see a closing bracket while expecting an element,
                 * this is a malformed array (for example, trailing comma).
                 * Do not free `elements` here; the `fail` label will perform
                 * cleanup once to avoid double-free/use-after-free.
                 */
                if (elements != NULL)
                {
                    goto fail;
                }

                pos++;
                mode = End;
            }
            else
            {
                Token *element = token_parse_value(s + pos, " \n\r\t],"); // [ \n\r\t\],]
                if (element != NULL)
                {
                    elements = append_token(elements, (Token *)element);
                    pos += TOKEN_SKIP(element); // token header helper
                    mode = Comma;
                }
                else
                {
                    goto fail;
                }
            }
            break;

        case Comma:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            {
                pos++;
            }
            else if (ch == ']')
            {
                pos++;
                mode = End;
            }
            else if (ch == ',')
            {
                pos++;
                mode = Elements;
            }
            else
            {
                goto fail;
            }
            break;

        case End:
            break;

        default:
            goto fail;
        }
    }

    ArrayToken *arraytokenp = (ArrayToken *)malloc(sizeof(ArrayToken));
    if (!arraytokenp)
    {
        if (elements)
            free_tokens(elements);
        goto fail;
    }
    arraytokenp->skip     = pos;
    arraytokenp->type     = TOKEN_ARRAY;
    arraytokenp->elements = elements;
    return arraytokenp;

fail:
    if (elements)
        free_tokens(elements);
    return NULL;
}

static Tokens *append_token(Tokens *tokensp, Token *tokenp)
{
    if (tokensp == NULL)
    {
        tokensp = (Tokens *)malloc(sizeof(Tokens));
        if (!tokensp)
            return NULL;
        tokensp->size   = 0;
        tokensp->tokens = (Token **)malloc(sizeof(Token *));
        if (!tokensp->tokens)
        {
            free(tokensp);
            return NULL;
        }
    }
    else
    {
        Token **newp = (Token **)realloc(tokensp->tokens, (1 + tokensp->size) * sizeof(Token *));
        if (!newp)
        {
            /* leave tokensp unchanged; caller should handle cleanup */
            return NULL;
        }
        tokensp->tokens = newp;
    }

    tokensp->tokens[tokensp->size++] = tokenp;
    return tokensp;
}
