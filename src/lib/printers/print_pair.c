#include <stdio.h>
#include <stdbool.h>
#include "../types/pairtoken.h"
#include "../json_internal.h"

int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    int written;
    if (suppress_leading_indent)
        written = json_snprintf(out, outsz, "PairToken { skip: %d,\n", tok->skip);
    else
        written = json_snprintf(out, outsz, "%*sPairToken { skip: %d,\n", indent, "", tok->skip);
    int total = written > 0 ? written : 0;
    if (tok->key)
    {
        extern int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz,
                                      bool suppress_leading_indent);
        written = json_snprintf(out + total, outsz - total, "%*skey: ", indent + 2, "");
        total += (written > 0 ? written : 0);
        written = print_string_token(tok->key, 0, out + total, outsz - total, true);
        total += (written > 0 ? written : 0);
    }
    if (tok->value)
    {
        extern int print_token(const Token *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
        int        value_type = ((const int32_t *)tok->value)[1];
        written               = json_snprintf(out + total, outsz - total, "%*svalue: ", indent + 2, "");
        total += (written > 0 ? written : 0);
        if (value_type == TOKEN_ARRAY || value_type == TOKEN_OBJECT)
            written = print_token(tok->value, indent + 2, out + total, outsz - total, true);
        else
            written = print_token(tok->value, 0, out + total, outsz - total, true);
        total += (written > 0 ? written : 0);
    }
    if (total < (int)outsz)
        total += json_snprintf(out + total, outsz - total, "%*s}\n", indent, "");
    return total;
}
