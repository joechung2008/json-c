#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "../src/lib/printers/print_array.h"
#include "../src/lib/parsers/value.h"
#include "../src/lib/token_free.h"

void test_print_array_token(void **state)
{
    (void)state;
    ArrayToken tok = {.skip = 0, .elements = NULL};
    char       buf[128];
    int        n = print_array_token(&tok, 2, buf, sizeof(buf), false);
    assert(strstr(buf, "ArrayToken") && n > 0);
    (void)n;
    print_array_token(&tok, 0, buf, sizeof(buf), true);
    assert(strncmp(buf, "ArrayToken", 10) == 0);
}

void test_print_array_with_array_element(void **state)
{
    (void)state;
    Token *t = token_parse("[[]]", false);
    assert(t != NULL);
    ArrayToken *tok = (ArrayToken *)t;
    char        buf[1024];
    int         n = print_array_token(tok, 0, buf, sizeof(buf), false);
    assert(n > 0 && strstr(buf, "ArrayToken") != NULL);
    (void)n;
    token_free(t);
}

void test_print_array_with_object_element(void **state)
{
    (void)state;
    Token *t = token_parse("[{}]", false);
    assert(t != NULL);
    ArrayToken *tok = (ArrayToken *)t;
    char        buf[1024];
    int         n = print_array_token(tok, 0, buf, sizeof(buf), false);
    assert(n > 0 && strstr(buf, "ArrayToken") != NULL && strstr(buf, "ObjectToken") != NULL);
    (void)n;
    token_free(t);
}

void test_print_array_with_nested_nonempty_array(void **state)
{
    (void)state;
    Token *t = token_parse("[[\"item\"]]", false);
    assert(t != NULL);
    ArrayToken *tok = (ArrayToken *)t;
    char        buf[2048];
    int         n = print_array_token(tok, 0, buf, sizeof(buf), false);
    assert(n > 0 && strstr(buf, "ArrayToken") != NULL && strstr(buf, "StringToken") != NULL);
    (void)n;
    token_free(t);
}

void test_print_array_with_nested_nonempty_object(void **state)
{
    (void)state;
    Token *t = token_parse("[{\"key\":\"value\"}]", false);
    assert(t != NULL);
    ArrayToken *tok = (ArrayToken *)t;
    char        buf[2048];
    int         n = print_array_token(tok, 0, buf, sizeof(buf), false);
    assert(n > 0 && strstr(buf, "ArrayToken") != NULL && strstr(buf, "ObjectToken") != NULL &&
           strstr(buf, "PairToken") != NULL);
    (void)n;
    token_free(t);
}