#include <stdio.h>
#include <stdbool.h>
#include "../types/numbertoken.h"
#include "../json_internal.h"

int print_number_token(const NumberToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    if (suppress_leading_indent)
        return json_snprintf(out, outsz, "NumberToken { skip: %d, value: %f }\n", tok->skip, tok->value);
    else
        return json_snprintf(out, outsz, "%*sNumberToken { skip: %d, value: %f }\n", indent, "", tok->skip, tok->value);
}
