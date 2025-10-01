#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "../types/objecttoken.h"
int print_object_token(const ObjectToken *tok, int indent, char *out, size_t outsz, bool suppress_leading_indent);
