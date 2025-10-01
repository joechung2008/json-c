#pragma once
#include <stdbool.h>
#include "../types/pairtoken.h"
int print_pair_token(const PairToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
