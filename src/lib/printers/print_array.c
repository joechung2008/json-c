#include <stdbool.h>
#include <stdio.h>

#include "../json_internal.h"
#include "../types/arraytoken.h"
#include "../types/token.h"

int print_array_token(const ArrayToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    int written;
    if (suppress_leading_indent)
        written = json_snprintf(out, outsz, "ArrayToken { skip: %d, elements: [\n", tok->skip);
    else
        written = json_snprintf(out, outsz, "%*sArrayToken { skip: %d, elements: [\n", indent, "", tok->skip);
    int total = written > 0 ? written : 0;
    if (tok->elements && tok->elements->tokens)
    {
        for (int i = 0; i < tok->elements->size && total < (int)outsz; ++i)
        {
            Token *elem = tok->elements->tokens[i];
            if (!elem)
                continue;
            // Recursively print the element. Assume a function: print_token(const Token*, int, char*, size_t)
            extern int print_token(const Token *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
            written = print_token(elem, indent + 2, out + total, outsz - total, false);
            total += (written > 0 ? written : 0);
        }
    }
    if (total < (int)outsz)
        total += json_snprintf(out + total, outsz - total, "%*s]}\n", indent, "");
    return total;
}
