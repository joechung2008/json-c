#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "../types/falsetoken.h"
int print_false_token(const FalseToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
