#pragma once

#include <stddef.h>
#include <stdlib.h>

#include "../types.h"

StringToken *json_parse_string(const char *s);
static long  parse_hexadecimal_code(const char *s);
