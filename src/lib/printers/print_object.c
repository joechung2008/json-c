#include <stdbool.h>
#include <stdio.h>

#include "../json_internal.h"
#include "../types/objecttoken.h"
#include "./print_pair.h"

int print_object_token(const ObjectToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    int written;
    if (suppress_leading_indent)
    {
        written = json_snprintf(out, outsz, "ObjectToken {\n");
    }
    else
    {
        written = json_snprintf(out, outsz, "%*sObjectToken {\n", indent, "");
    }

    int total = written > 0 ? written : 0;
    written   = json_snprintf(out + total, outsz - total, "%*smembers: [\n", indent + 4, "");
    total += written > 0 ? written : 0;
    if (tok->members && tok->members->pairs)
    {
        for (int i = 0; i < tok->members->size && total < (int)outsz; ++i)
        {
            PairToken *pair = tok->members->pairs[i];
            if (!pair)
            {
                continue;
            }

            written = print_pair_token(pair, indent + 8, out + total, outsz - total, false);
            total += written > 0 ? written : 0;
            if (i < tok->members->size - 1)
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
