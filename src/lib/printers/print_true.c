#include <stdio.h>
#include "../types/truetoken.h"

int print_true_token(const TrueToken *tok, int indent, char *out, size_t outsz)
{
    return snprintf(out, outsz, "%*sTrueToken { skip: %d }\n", indent, "", tok->skip);
}
