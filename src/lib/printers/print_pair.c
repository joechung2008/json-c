#include <stdbool.h>
#include <stdio.h>

#include "../json_internal.h"
#include "../types/pairtoken.h"
#include "./print_string.h"
#include "./print_token.h"

int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    int written;
    if (suppress_leading_indent)
    {
        written = json_snprintf(out, outsz, "PairToken {\n");
    }
    else
    {
        written = json_snprintf(out, outsz, "%*sPairToken {\n", indent, "");
    }

    int total = written > 0 ? written : 0;
    if (tok->key)
    {
        written = json_snprintf(out + total, outsz - total, "%*skey: ", indent + 4, "");
        total += (written > 0 ? written : 0);
        written = print_string_token(tok->key, 0, out + total, outsz - total, true);
        total += (written > 0 ? written : 0);
    }

    if (tok->value)
    {
        written = json_snprintf(out + total, outsz - total, ",\n%*svalue: ", indent + 4, "");
        total += (written > 0 ? written : 0);
        written = print_token(tok->value, indent + 4, out + total, outsz - total, true);
        total += (written > 0 ? written : 0);
    }

    written = json_snprintf(out + total, outsz - total, "\n%*s}", indent, "");
    total += (written > 0 ? written : 0);
    return total;
}
