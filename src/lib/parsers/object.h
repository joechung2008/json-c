#pragma once

#include <stddef.h>

#include "../types.h"

ObjectToken       *json_parse_object(const char *s);
static PairTokens *append_pair(PairTokens *, PairToken *);
