#pragma once
#include <stdbool.h>
#include "../types/arraytoken.h"
int print_array_token(const ArrayToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
