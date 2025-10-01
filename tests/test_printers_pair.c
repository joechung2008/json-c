#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/printers/print_pair.h"

void test_print_pair_token()
{
    PairToken tok = {.skip = 0, .key = NULL, .value = NULL};
    char      buf[128];
    int       n = print_pair_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "PairToken") && n > 0);
    print_pair_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "PairToken", 9) == 0);
}