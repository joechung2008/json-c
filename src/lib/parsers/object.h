#pragma once

#include <stddef.h>

#include "../types.h"

ObjectToken       *json_parse_object(char *);
static PairTokens *append_pair(PairTokens *, PairToken *);
