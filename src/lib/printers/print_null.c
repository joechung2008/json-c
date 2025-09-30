#include <stdio.h>
#include "../types/nulltoken.h"

int print_null_token(const NullToken *tok, int indent, char *out, size_t outsz)
{
    return snprintf(out, outsz, "%*sNullToken { skip: %d, type: %d }\n", indent, "", tok->skip, tok->type);
}
