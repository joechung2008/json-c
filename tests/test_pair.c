#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

/* Test parsing of a simple pair inside an object */
void test_parse_pair_simple(void **state)
{
    (void)state;
    const char *input = "{\"a\": 1}";
    void       *v     = json_parse(input);
    assert_non_null(v);
    const char *txt = json_text((const json_value_t *)v);
    assert_non_null(txt);
    /* we expect the textual representation to contain the key "a" */
    assert_non_null(strstr(txt, "\"a\""));
    json_free((json_value_t *)v);
}

void test_parse_pair_key_and_value_string(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"name\": \"John\"}");
    assert_non_null(v);

    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "name");

    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    const char *s = json_value_get_string(val);
    assert_string_equal(s, "John");
    json_value_free_wrapper(val);

    json_free(v);
}

void test_parse_pair_key_and_value_number(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"age\": 42}");
    assert_non_null(v);

    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "age");

    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    double d = 0;
    assert_true(json_value_get_number(val, &d));
    assert_true(d == 42.0);
    json_value_free_wrapper(val);

    json_free(v);
}

void test_parse_pair_key_and_value_bool(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"active\": true}");
    assert_non_null(v);

    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "active");

    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    int b = -1;
    assert_true(json_value_get_bool(val, &b));
    assert_int_equal(b, 1);
    json_value_free_wrapper(val);

    json_free(v);
}

void test_parse_pair_key_and_value_null(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"data\": null}");
    assert_non_null(v);

    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "data");

    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    assert_int_equal(json_value_type(val), JSON_NULL);
    json_value_free_wrapper(val);

    json_free(v);
}

void test_parse_pair_value_array_and_object_types(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"items\": [1,2,3]}");
    assert_non_null(v);
    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "items");
    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    assert_int_equal(json_value_type(val), JSON_ARRAY);
    json_value_free_wrapper(val);
    json_free(v);

    v = json_parse("{\"nested\": {\"inner\": \"value\"}}");
    assert_non_null(v);
    k = json_object_key(v, 0);
    assert_string_equal(k, "nested");
    val = json_object_value(v, 0);
    assert_non_null(val);
    assert_int_equal(json_value_type(val), JSON_OBJECT);
    json_value_free_wrapper(val);
    json_free(v);
}

void test_parse_pair_whitespace_handling(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"key\"  :  \"value\"}");
    assert_non_null(v);
    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "key");
    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    const char *s = json_value_get_string(val);
    assert_string_equal(s, "value");
    json_value_free_wrapper(val);
    json_free(v);

    v = json_parse("{  \"key\": \"value\"  }");
    assert_non_null(v);
    k = json_object_key(v, 0);
    assert_string_equal(k, "key");
    val = json_object_value(v, 0);
    assert_non_null(val);
    s = json_value_get_string(val);
    assert_string_equal(s, "value");
    json_value_free_wrapper(val);
    json_free(v);
}

void test_parse_pair_errors(void **state)
{
    (void)state;
    json_value_t *v;

    v = json_parse("{\"key\" \"value\"}");
    assert_null(v);

    v = json_parse("{\"key\"; \"value\"}");
    assert_null(v);

    v = json_parse("{key\": \"value\"}");
    assert_null(v);

    v = json_parse("{\"key\": invalid}");
    assert_null(v);
}

void test_parse_multiple_pairs_and_commas(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"key1\": \"value1\", \"key2\": \"value2\"}");
    assert_non_null(v);

    const char *k0 = json_object_key(v, 0);
    assert_string_equal(k0, "key1");
    json_value_t *val0 = json_object_value(v, 0);
    assert_non_null(val0);
    const char *s0 = json_value_get_string(val0);
    assert_string_equal(s0, "value1");
    json_value_free_wrapper(val0);

    const char *k1 = json_object_key(v, 1);
    assert_string_equal(k1, "key2");
    json_value_t *val1 = json_object_value(v, 1);
    assert_non_null(val1);
    const char *s1 = json_value_get_string(val1);
    assert_string_equal(s1, "value2");
    json_value_free_wrapper(val1);

    json_free(v);
}
