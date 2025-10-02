#include <stdbool.h>
#include <stdio.h>

#include "../json_internal.h"
#include "../types/arraytoken.h"
#include "../types/token.h"
#include "./print_token.h"

int print_array_token(const ArrayToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    int written;
    if (suppress_leading_indent)
    {
        written = json_snprintf(out, outsz, "ArrayToken {\n");
    }
    else
    {
        written = json_snprintf(out, outsz, "%*sArrayToken {\n", indent, "");
    }

    int total = written > 0 ? written : 0;
    written   = json_snprintf(out + total, outsz - total, "%*selements: [\n", indent + 4, "");
    total += written > 0 ? written : 0;
    if (tok->elements && tok->elements->tokens)
    {
        for (int i = 0; i < tok->elements->size && total < (int)outsz; ++i)
        {
            Token *elem = tok->elements->tokens[i];
            if (!elem)
            {
                continue;
            }

            written = print_token(elem, indent + 8, out + total, outsz - total, false);
            total += (written > 0 ? written : 0);
            if (i < tok->elements->size - 1)
            {
                written = json_snprintf(out + total, outsz - total, ",\n");
                total += written > 0 ? written : 0;
            }
            else
            {
                written = json_snprintf(out + total, outsz - total, "\n");
                total += written > 0 ? written : 0;
            }
        }
    }

    written = json_snprintf(out + total, outsz - total, "%*s]\n", indent + 4, "");
    total += written > 0 ? written : 0;
    written = json_snprintf(out + total, outsz - total, "%*s}", indent, "");
    total += written > 0 ? written : 0;
    return total;
}
