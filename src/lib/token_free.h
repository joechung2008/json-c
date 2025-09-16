// token_free.h
#pragma once
#include "types.h"

void token_free(void *tok);
void free_tokens(struct Tokens *t);
void free_pairtokens(struct PairTokens *p);
