#include <stdio.h>
#include "../types/pairtoken.h"

int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz)
{
    int written = snprintf(out, outsz, "%*sPairToken { skip: %d, type: %d,\n", indent, "", tok->skip, tok->type);
    int total = written > 0 ? written : 0;
    if (tok->key) {
        extern int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz);
        written = print_string_token(tok->key, indent + 2, out + total, outsz - total);
        total += (written > 0 ? written : 0);
    }
    if (tok->value) {
        extern int print_token(const Token *tok, int indent, char *out, size_t outsz);
        written = print_token(tok->value, indent + 2, out + total, outsz - total);
        total += (written > 0 ? written : 0);
    }
    if (total < (int)outsz)
        total += snprintf(out + total, outsz - total, "%*s}\n", indent, "");
    return total;
}
