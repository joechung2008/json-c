#include <stdio.h>
#include "../types/nulltoken.h"
#include "../json_internal.h"

int print_null_token(const NullToken *tok, int indent, char *out, size_t outsz)
{
    return json_snprintf(out, outsz, "%*sNullToken { skip: %d }\n", indent, "", tok->skip);
}
