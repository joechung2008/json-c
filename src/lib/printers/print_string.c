#include <stdbool.h>
#include <stdio.h>
#include "../types/stringtoken.h"
#include "../json_internal.h"

int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    if (suppress_leading_indent)
        return json_snprintf(out, outsz, "StringToken { skip: %d, value: \"%s\" }\n", tok->skip, tok->value);
    else
        return json_snprintf(out, outsz, "%*sStringToken { skip: %d, value: \"%s\" }\n", indent, "", tok->skip,
                             tok->value);
}
