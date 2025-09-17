#include <string.h>
#include <stdlib.h>

#include "./pair.h"
#include "./string.h"
#include "./value.h"
#include "../token_free.h"
#include "../token_helpers.h"

// Token header contract: every token begins with two ints (skip, type).
// Use TOKEN_SKIP(t) and TOKEN_TYPE(t) from src/lib/token_helpers.h to access these fields.
PairToken *json_parse_pair(const char *s)
{
    enum
    {
        Scanning,
        Key,
        Colon,
        Value,
        End
    };

    int          mode  = Scanning;
    int          pos   = 0;
    StringToken *key   = NULL;
    Token       *value = NULL;

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
            else if (ch == '"')
            {
                mode = Key;
            }
            else
            {
                goto fail;
            }
            break;

        case Key:
            key = json_parse_string(s + pos);
            if (key != NULL)
            {
                pos += key->skip;
                mode = Colon;
            }
            else
            {
                goto fail;
            }
            break;

        case Colon:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            {
                pos++;
            }
            else if (ch == ':')
            {
                pos++;
                mode = Value;
            }
            else
            {
                goto fail;
            }
            break;

        case Value:
            value = token_parse_value(s + pos, " \n\r\t},"); // [ \n\r\t\},]
            if (value != NULL)
            {
                pos += token_get_skip((void *)value); // token header helper
                mode = End;
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

    PairToken *pairtokenp = (PairToken *)malloc(sizeof(PairToken));
    if (!pairtokenp)
    {
        goto fail;
    }
    pairtokenp->skip  = pos;
    pairtokenp->type  = TOKEN_PAIR;
    pairtokenp->key   = key;
    pairtokenp->value = value;
    return pairtokenp;

fail:
    if (key)
        token_free(key);
    if (value)
        token_free(value);
    return NULL;
}
