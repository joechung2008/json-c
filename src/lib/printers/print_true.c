#include <stdio.h>
#include "../types/truetoken.h"
#include "../json_internal.h"

int print_true_token(const TrueToken *tok, int indent, char *out, size_t outsz)
{
    return json_snprintf(out, outsz, "%*sTrueToken { skip: %d }\n", indent, "", tok->skip);
}
