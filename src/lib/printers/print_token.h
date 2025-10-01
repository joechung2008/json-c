#ifndef PRINT_TOKEN_H
#define PRINT_TOKEN_H

#include <stdio.h>
#include <stdbool.h>
#include "../types/token.h"

int print_token(const Token *token, int indent, char *out, size_t outsz, bool suppress_leading_indent);

#endif // PRINT_TOKEN_H
