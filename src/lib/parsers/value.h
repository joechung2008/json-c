#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "../types.h"

Token *token_parse(char *, bool);
Token *token_parse_value(char *, char *);
