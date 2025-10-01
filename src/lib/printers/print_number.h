#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "../types/numbertoken.h"
int print_number_token(const NumberToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
