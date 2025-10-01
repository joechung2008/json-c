#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/lib/printers/print_false.h"
#include "../src/lib/printers/print_null.h"
#include "../src/lib/printers/print_true.h"

void test_print_null_token(void **state)
{
    (void)state;
    NullToken tok = {.skip = 0};
    char      buf[128];
    int       n = print_null_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "NullToken") && n > 0);
    print_null_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "NullToken", 9) == 0);
}

void test_print_false_token(void **state)
{
    (void)state;
    FalseToken tok = {.skip = 0};
    char       buf[128];
    int        n = print_false_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "FalseToken") && n > 0);
    print_false_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "FalseToken", 10) == 0);
}

void test_print_true_token(void **state)
{
    (void)state;
    TrueToken tok = {.skip = 0};
    char      buf[128];
    int       n = print_true_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "TrueToken") && n > 0);
    print_true_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "TrueToken", 9) == 0);
}
