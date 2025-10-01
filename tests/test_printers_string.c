#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/printers/print_string.h"

void test_print_string_token()
{
    StringToken tok = {.skip = 0, .value = "hello"};
    char        buf[128];
    int         n = print_string_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "StringToken") && n > 0);
    n = print_string_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "StringToken", 11) == 0);
}