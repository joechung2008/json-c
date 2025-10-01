#include <stdio.h>
#include "../types/numbertoken.h"
#include "../json_internal.h"

int print_number_token(const NumberToken *tok, int indent, char *out, size_t outsz)
{
    return json_snprintf(out, outsz, "%*sNumberToken { skip: %d, value: %f }\n", indent, "", tok->skip, tok->value);
}
