#include <stdio.h>
#include <string.h>
#include "../types/arraytoken.h"
#include "../types/token.h"

int print_array_token(const ArrayToken *tok, int indent, char *out, size_t outsz)
{
    int written = snprintf(out, outsz, "%*sArrayToken { skip: %d, type: %d, elements: [\n", indent, "", tok->skip, tok->type);
    int total = written > 0 ? written : 0;
    if (tok->elements && tok->elements->tokens) {
        for (int i = 0; i < tok->elements->size && total < (int)outsz; ++i) {
            Token *elem = tok->elements->tokens[i];
            if (!elem) continue;
            // Recursively print the element. Assume a function: print_token(const Token*, int, char*, size_t)
            extern int print_token(const Token *tok, int indent, char *out, size_t outsz);
            written = print_token(elem, indent + 2, out + total, outsz - total);
            total += (written > 0 ? written : 0);
        }
    }
    if (total < (int)outsz)
        total += snprintf(out + total, outsz - total, "%*s]}\n", indent, "");
    return total;
}
