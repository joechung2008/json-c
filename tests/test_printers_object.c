#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../src/lib/printers/print_object.h"
#include "../src/lib/parsers/value.h"
#include "../src/lib/token_free.h"

void test_print_object_token()
{
    ObjectToken tok = {.skip = 0, .members = NULL};
    char        buf[128];
    int         n = print_object_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "ObjectToken") && n > 0);
    n = print_object_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "ObjectToken", 11) == 0);
}

void test_print_object_with_pair_array_value()
{
    Token *t = token_parse("{\"key\":[]}", false);
    assert(t != NULL);
    ObjectToken *tok = (ObjectToken *)t;
    char         buf[1024];
    int          n = print_object_token(tok, 0, buf, sizeof(buf), false);
    assert(n > 0 && strstr(buf, "ObjectToken") != NULL && strstr(buf, "ArrayToken") != NULL);
    token_free(t);
}

void test_print_object_with_pair_object_value()
{
    Token *t = token_parse("{\"key\":{}}", false);
    assert(t != NULL);
    ObjectToken *tok = (ObjectToken *)t;
    char         buf[1024];
    int          n = print_object_token(tok, 0, buf, sizeof(buf), false);
    assert(n > 0 && strstr(buf, "ObjectToken") != NULL);
    token_free(t);
}