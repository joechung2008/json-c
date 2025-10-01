#include <stdio.h>
#include "../types/stringtoken.h"
#include "../json_internal.h"

int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz)
{
    return json_snprintf(out, outsz, "%*sStringToken { skip: %d, value: \"%s\" }\n", indent, "", tok->skip, tok->value);
}
