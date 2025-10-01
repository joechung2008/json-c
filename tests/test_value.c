#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

void test_parse_simple_value(void **state)
{
    (void)state;
    const char   *txt = "{\"ok\":true}";
    json_value_t *v   = json_parse(txt);
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_OBJECT);
    assert_int_equal((int)json_object_size(v), 1);
    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "ok");
    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    assert_int_equal(json_value_type(val), JSON_TRUE);
    bool b = false;
    assert_true(json_value_get_bool(val, &b));
    assert_true(b == true);
    json_value_free_wrapper(val);
    json_free(v);
}

void test_parse_value_booleans_null(void **state)
{
    (void)state;
    void *v;

    v = json_parse("false");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("true");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("null");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_value_number_and_string(void **state)
{
    (void)state;
    void *v;

    v = json_parse("123");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("-45.67");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("\"hello world\"");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_value_array_object_whitespace(void **state)
{
    (void)state;
    void *v;

    v = json_parse("[1,2,3]");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("{\"a\":1,\"b\":false}");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("   true   ");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_array_and_values(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[1,2,3]");
    assert_non_null(v);
    json_type_t t = json_value_type(v);
    assert_int_equal(t, JSON_ARRAY);
    size_t n = json_array_length(v);
    assert_int_equal((int)n, 3);

    json_value_t *e0 = json_array_get(v, 0);
    assert_non_null(e0);
    double val = 0;
    assert_true(json_value_get_number(e0, &val));
    assert_true(val == 1.0);
    json_value_free_wrapper(e0);

    json_free(v);
}

void test_parse_value_array_type(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[1,2,3]");
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_ARRAY);
    json_free(v);
}

void test_parse_value_array_values(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[1,2,3]");
    assert_non_null(v);
    size_t n = json_array_length(v);
    assert_int_equal((int)n, 3);

    json_value_t *e0 = json_array_get(v, 0);
    assert_non_null(e0);
    double val = 0;
    assert_true(json_value_get_number(e0, &val));
    assert_true(val == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(v, 1);
    assert_non_null(e1);
    assert_true(json_value_get_number(e1, &val));
    assert_true(val == 2.0);
    json_value_free_wrapper(e1);

    json_value_t *e2 = json_array_get(v, 2);
    assert_non_null(e2);
    assert_true(json_value_get_number(e2, &val));
    assert_true(val == 3.0);
    json_value_free_wrapper(e2);

    json_free(v);
}

void test_parse_invalid_input(void **state)
{
    (void)state;
    json_value_t *v = json_parse("invalid");
    assert_null(v);
}

void test_parse_empty_input(void **state)
{
    (void)state;
    json_value_t *v = json_parse("");
    assert_null(v);
}

void test_parse_malformed_false_literal(void **state)
{
    (void)state;
    json_value_t *v = json_parse("fals");
    assert_null(v);
}

void test_parse_malformed_true_literal(void **state)
{
    (void)state;
    json_value_t *v = json_parse("tru");
    assert_null(v);
}

void test_parse_malformed_null_literal(void **state)
{
    (void)state;
    json_value_t *v = json_parse("nul");
    assert_null(v);
}

void test_parse_unexpected_character(void **state)
{
    (void)state;
    json_value_t *v = json_parse("@");
    assert_null(v);
}
