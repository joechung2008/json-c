#include <stdio.h>
#include "../types/pairtoken.h"

int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz)
{
    int written = snprintf(out, outsz, "%*sPairToken { skip: %d,\n", indent, "", tok->skip);
    int total = written > 0 ? written : 0;
    if (tok->key) {
        extern int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz);
        written = snprintf(out + total, outsz - total, "%*skey: ", indent + 2, "");
        total += (written > 0 ? written : 0);
        written = print_string_token(tok->key, 0, out + total, outsz - total);
        total += (written > 0 ? written : 0);
    }
    if (tok->value) {
        extern int print_token(const Token *tok, int indent, char *out, size_t outsz);
        written = snprintf(out + total, outsz - total, "%*svalue: ", indent + 2, "");
        total += (written > 0 ? written : 0);
        written = print_token(tok->value, 0, out + total, outsz - total);
        total += (written > 0 ? written : 0);
    }
    if (total < (int)outsz)
        total += snprintf(out + total, outsz - total, "%*s}\n", indent, "");
    return total;
}
