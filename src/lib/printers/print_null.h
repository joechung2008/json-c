#pragma once
#include <stdbool.h>
#include "../types/nulltoken.h"
int print_null_token(const NullToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
