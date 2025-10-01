#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "../types/token.h"
int print_token(const Token *token, int indent, char *out, size_t outsz, bool suppress_leading_indent);
