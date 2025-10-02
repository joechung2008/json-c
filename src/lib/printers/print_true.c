#include <stdbool.h>
#include <stdio.h>

#include "../json_internal.h"
#include "../types/truetoken.h"

int print_true_token(const TrueToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    if (suppress_leading_indent)
    {
        return json_snprintf(out, outsz, "TrueToken { skip: %d }", tok->skip);
    }
    else
    {
        return json_snprintf(out, outsz, "%*sTrueToken { skip: %d }", indent, "", tok->skip);
    }
}
