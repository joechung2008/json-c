#include <string.h>
#include <stdlib.h>

#include "array.h"
#include "number.h"
#include "object.h"
#include "string.h"
#include "value.h"

Token *token_parse(char *s, bool strict)
{
    if (strict)
    {
        return token_parse_value(s, NULL);
    }
    else
    {
        return token_parse_value(s, " \n\r\t");
    }
}

Token *token_parse_value(char *s, char *delimiters)
{
    enum
    {
        Scanning,
        Array,
        False,
        Null,
        Number,
        Object,
        String,
        True,
        End
    };

    int    mode   = Scanning;
    int    pos    = 0;
    Token *tokenp = NULL;

    while (pos < strlen(s) && mode != End)
    {
        char ch = s[pos];

        switch (mode)
        {
        case Scanning:
            if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
            { // [ \n\r\t]
                pos++;
            }
            else if (ch == '[')
            {
                mode = Array;
            }
            else if (ch == 'f')
            {
                mode = False;
            }
            else if (ch == 'n')
            {
                mode = Null;
            }
            else if (ch == '-' || (ch >= '0' && ch <= '9'))
            { // [-\d]
                mode = Number;
            }
            else if (ch == '{')
            {
                mode = Object;
            }
            else if (ch == '"')
            {
                mode = String;
            }
            else if (ch == 't')
            {
                mode = True;
            }
            else
            {
                return NULL;
            }
            break;

        case Array:
            ArrayToken *arraytokenp = json_parse_array(s + pos);
            if (arraytokenp != NULL)
            {
                arraytokenp->skip += pos;
                tokenp = arraytokenp;
                mode   = End;
                break;
            }
            return NULL;

        case False:
            if (strncmp(s + pos, "false", 5) == 0)
            {
                FalseToken *falsetokenp = (FalseToken *)malloc(sizeof(FalseToken));
                if (falsetokenp != NULL)
                {
                    falsetokenp->skip  = pos + 5;
                    falsetokenp->type  = TOKEN_FALSE;
                    falsetokenp->value = false;
                    tokenp             = falsetokenp;
                    mode               = End;
                    break;
                }
            }
            return NULL;

        case Null:
            if (strncmp(s + pos, "null", 4) == 0)
            {
                NullToken *nulltokenp = (NullToken *)malloc(sizeof(NullToken));
                if (nulltokenp != NULL)
                {
                    nulltokenp->skip = pos + 4;
                    nulltokenp->type = TOKEN_NULL;
                    tokenp           = nulltokenp;
                    mode             = End;
                    break;
                }
            }
            return NULL;

        case Number:
            NumberToken *numbertokenp = json_parse_number(s + pos, delimiters);
            if (numbertokenp != NULL)
            {
                numbertokenp->skip += pos;
                tokenp = numbertokenp;
                mode   = End;
                break;
            }
            return NULL;

        case Object:
            ObjectToken *objecttokenp = json_parse_object(s + pos);
            if (objecttokenp != NULL)
            {
                objecttokenp->skip += pos;
                tokenp = objecttokenp;
                mode   = End;
                break;
            }
            return NULL;

        case String:
            StringToken *stringtokenp = json_parse_string(s + pos);
            if (stringtokenp != NULL)
            {
                stringtokenp->skip += pos;
                tokenp = stringtokenp;
                mode   = End;
                break;
            }
            return NULL;

        case True:
            if (strncmp(s + pos, "true", 4) == 0)
            {
                TrueToken *truetokenp = (TrueToken *)malloc(sizeof(TrueToken));
                if (truetokenp != NULL)
                {
                    truetokenp->skip  = pos + 4;
                    truetokenp->type  = TOKEN_TRUE;
                    truetokenp->value = true;
                    tokenp            = truetokenp;
                    mode              = End;
                    break;
                }
            }
            return NULL;

        case End:
            break;

        default:
            return NULL;
        }
    }

    return tokenp;
}
