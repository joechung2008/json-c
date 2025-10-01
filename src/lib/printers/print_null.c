#include <stdbool.h>
#include <stdio.h>
#include "../types/nulltoken.h"
#include "../json_internal.h"

int print_null_token(const NullToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    if (suppress_leading_indent)
        return json_snprintf(out, outsz, "NullToken { skip: %d }\n", tok->skip);
    else
        return json_snprintf(out, outsz, "%*sNullToken { skip: %d }\n", indent, "", tok->skip);
}
