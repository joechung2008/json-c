#include <stdbool.h>

#include "../json_internal.h"
#include "../types/arraytoken.h"
#include "../types/falsetoken.h"
#include "../types/nulltoken.h"
#include "../types/numbertoken.h"
#include "../types/objecttoken.h"
#include "../types/pairtoken.h"
#include "../types/token.h"
#include "../types/truetoken.h"
#include "./print_array.h"
#include "./print_false.h"
#include "./print_null.h"
#include "./print_number.h"
#include "./print_object.h"
#include "./print_pair.h"
#include "./print_string.h"
#include "./print_token.h"
#include "./print_true.h"

int print_token(const Token *token, int indent, char *out, size_t outsz, bool suppress_leading_indent)
{
    if (!token)
    {
        if (suppress_leading_indent)
            return json_snprintf(out, outsz, "(null token)\n");
        else
            return json_snprintf(out, outsz, "%*s(null token)\n", indent, "");
    }
    int type = ((const int32_t *)token)[1]; // type is second int field
    switch (type)
    {
    case TOKEN_OBJECT:
        return print_object_token((const ObjectToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_ARRAY:
        return print_array_token((const ArrayToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_PAIR:
        return print_pair_token((const PairToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_STRING:
        return print_string_token((const StringToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_NUMBER:
        return print_number_token((const NumberToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_TRUE:
        return print_true_token((const TrueToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_FALSE:
        return print_false_token((const FalseToken *)token, indent, out, outsz, suppress_leading_indent);
    case TOKEN_NULL:
        return print_null_token((const NullToken *)token, indent, out, outsz, suppress_leading_indent);
    default:
        if (suppress_leading_indent)
            return json_snprintf(out, outsz, "(unknown token type)\n");
        else
            return json_snprintf(out, outsz, "%*s(unknown token type)\n", indent, "");
    }
}
