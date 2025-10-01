#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "../types/truetoken.h"
int print_true_token(const TrueToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
