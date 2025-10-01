#pragma once
#include <stdbool.h>
#include "../types/stringtoken.h"
int print_string_token(const StringToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
