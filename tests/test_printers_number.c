#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/printers/print_number.h"

void test_print_number_token(void **state)
{
    (void)state;
    NumberToken tok = {.skip = 0, .value = 42.0};
    char        buf[128];
    int         n = print_number_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "NumberToken") && n > 0);
    (void)n;
    print_number_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "NumberToken", 11) == 0);
}