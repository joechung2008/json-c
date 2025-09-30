#include <stdio.h>
#include "../types/stringtoken.h"

int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz)
{
    return snprintf(out, outsz, "%*sStringToken { skip: %d, type: %d, value: \"%s\" }\n", indent, "", tok->skip,
                    tok->type, tok->value);
}
