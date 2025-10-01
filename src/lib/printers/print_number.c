#include <stdio.h>
#include "../types/numbertoken.h"

int print_number_token(const NumberToken *tok, int indent, char *out, size_t outsz)
{
    return snprintf(out, outsz, "%*sNumberToken { skip: %d, value: %f }\n", indent, "", tok->skip, tok->value);
}
