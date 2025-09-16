#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "../types.h"

Token *token_parse(const char *s, bool strict);
Token *token_parse_value(const char *s, const char *delimiters);
