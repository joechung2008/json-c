#include <stdio.h>
#include "../types/falsetoken.h"

int print_false_token(const FalseToken *tok, int indent, char *out, size_t outsz)
{
    return snprintf(out, outsz, "%*sFalseToken { skip: %d, type: %d }\n", indent, "", tok->skip, tok->type);
}
