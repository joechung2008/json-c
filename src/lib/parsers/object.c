#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../token_free.h"
#include "./object.h"
#include "./pair.h"

static PairTokens *append_pair(PairTokens *, PairToken *);

ObjectToken *json_parse_object(const char *s)
{
    enum
    {
        Scanning,
        Pair,
        Delimiter,
        End
    };

    int         mode    = Scanning;
    int         pos     = 0;
    PairTokens *members = NULL;

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
            else if (ch == '{')
            {
                pos++;
                mode = Pair;
            }
            else
            {
                goto fail;
            }
            break;

        case Pair:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            {
                pos++;
            }
            else if (ch == '}')
            {
                /* If we see a closing brace while expecting a pair, this is a
                 * malformed object (e.g. a trailing comma). Do not free
                 * `members` here — the `fail` label will handle cleanup once.
                 */
                if (members != NULL)
                {
                    goto fail;
                }

                pos++;
                mode = End;
            }
            else
            {
                PairToken *member = json_parse_pair(s + pos);
                if (member != NULL)
                {
                    members = append_pair(members, member);
                    pos += member->skip;
                    mode = Delimiter;
                }
                else
                {
                    goto fail;
                }
            }
            break;

        case Delimiter:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            {
                pos++;
            }
            else if (ch == ',')
            {
                pos++;
                mode = Pair;
            }
            else if (ch == '}')
            {
                pos++;
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

    ObjectToken *objecttokenp = (ObjectToken *)malloc(sizeof(ObjectToken));
    if (!objecttokenp)
    {
        if (members)
            free_pairtokens(members);
        goto fail;
    }
    objecttokenp->skip    = pos;
    objecttokenp->type    = TOKEN_OBJECT;
    objecttokenp->members = members;
    return objecttokenp;

fail:
    if (members)
        free_pairtokens(members);
    return NULL;
}

static PairTokens *append_pair(PairTokens *tokensp, PairToken *tokenp)
{
    if (tokensp == NULL)
    {
        tokensp = (PairTokens *)malloc(sizeof(PairTokens));
        if (!tokensp)
            return NULL;
        tokensp->size  = 0;
        tokensp->pairs = (PairToken **)malloc(sizeof(PairToken *));
        if (!tokensp->pairs)
        {
            free(tokensp);
            return NULL;
        }
    }
    else
    {
        PairToken **tmp = (PairToken **)realloc(tokensp->pairs, (1 + tokensp->size) * sizeof(PairToken *));
        if (!tmp)
        {
            return NULL;
        }
        tokensp->pairs = tmp;
    }

    tokensp->pairs[tokensp->size++] = tokenp;
    return tokensp;
}
