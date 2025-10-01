#include <stdio.h>
#include "../types/falsetoken.h"
#include "../json_internal.h"

int print_false_token(const FalseToken *tok, int indent, char *out, size_t outsz)
{
    return json_snprintf(out, outsz, "%*sFalseToken { skip: %d }\n", indent, "", tok->skip);
}
