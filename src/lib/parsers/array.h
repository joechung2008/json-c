#pragma once

#include <stddef.h>

#include "../types.h"

ArrayToken    *json_parse_array(const char *s);
static Tokens *append_token(Tokens *, Token *);
