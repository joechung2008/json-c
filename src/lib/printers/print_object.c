#include <stdio.h>
#include "../types/objecttoken.h"

int print_object_token(const ObjectToken *tok, int indent, char *out, size_t outsz)
{
    int written = snprintf(out, outsz, "%*sObjectToken { skip: %d, pairs: [\n", indent, "", tok->skip);
    int total   = written > 0 ? written : 0;
    if (tok->members && tok->members->pairs)
    {
        for (int i = 0; i < tok->members->size && total < (int)outsz; ++i)
        {
            PairToken *pair = tok->members->pairs[i];
            if (!pair)
                continue;
            extern int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz);
            written = print_pair_token(pair, indent + 2, out + total, outsz - total);
            total += (written > 0 ? written : 0);
        }
    }
    if (total < (int)outsz)
        total += snprintf(out + total, outsz - total, "%*s]}\n", indent, "");
    return total;
}
