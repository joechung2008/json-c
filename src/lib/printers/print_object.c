#include <stdio.h>
#include "../types/objecttoken.h"
#include "../json_internal.h"
#include <stdbool.h>

int print_object_token(const ObjectToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    int written;
    if (suppress_leading_indent)
        written = json_snprintf(out, outsz, "ObjectToken { skip: %d, pairs: [\n", tok->skip);
    else
        written = json_snprintf(out, outsz, "%*sObjectToken { skip: %d, pairs: [\n", indent, "", tok->skip);
    int total = written > 0 ? written : 0;
    if (tok->members && tok->members->pairs)
    {
        for (int i = 0; i < tok->members->size && total < (int)outsz; ++i)
        {
            PairToken *pair = tok->members->pairs[i];
            if (!pair)
                continue;
            extern int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz,
                                        bool suppress_leading_indent);
            written = print_pair_token(pair, indent + 2, out + total, outsz - total, false);
            total += (written > 0 ? written : 0);
        }
    }
    if (total < (int)outsz)
        total += json_snprintf(out + total, outsz - total, "%*s]}\n", indent, "");
    return total;
}
