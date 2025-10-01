#include <stdio.h>
#include <stdbool.h>
#include "../types/falsetoken.h"
#include "../json_internal.h"

int print_false_token(const FalseToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    if (suppress_leading_indent)
        return json_snprintf(out, outsz, "FalseToken { skip: %d }\n", tok->skip);
    else
        return json_snprintf(out, outsz, "%*sFalseToken { skip: %d }\n", indent, "", tok->skip);
}
