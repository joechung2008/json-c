#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

void test_parse_empty_object(void **state)
{
    (void)state;
    const char   *txt = "{}";
    json_value_t *v   = json_parse(txt);
    assert_non_null(v);
    assert_string_equal(json_text(v), txt);
    json_free(v);
}

void test_parse_object_single_and_multiple(void **state)
{
    (void)state;
    void *v;

    v = json_parse("{\"a\":1}");
    assert_non_null(v);
    /* textual checks for key presence */
    assert_non_null(strstr(json_text((const json_value_t *)v), "\"a\""));
    json_free((json_value_t *)v);

    v = json_parse("{\"x\":true,\"y\":null,\"z\":\"abc\"}");
    assert_non_null(v);
    assert_non_null(strstr(json_text((const json_value_t *)v), "\"x\""));
    assert_non_null(strstr(json_text((const json_value_t *)v), "\"y\""));
    assert_non_null(strstr(json_text((const json_value_t *)v), "\"z\""));
    json_free((json_value_t *)v);
}

void test_parse_object_nested_and_mixed(void **state)
{
    (void)state;
    void *v;

    v = json_parse("{\"outer\":{\"inner\":42}}");
    assert_non_null(v);
    assert_non_null(strstr(json_text((const json_value_t *)v), "outer"));
    assert_non_null(strstr(json_text((const json_value_t *)v), "inner"));
    json_free((json_value_t *)v);

    v = json_parse("{\"arr\":[1,2,3]}");
    assert_non_null(v);
    assert_non_null(strstr(json_text((const json_value_t *)v), "arr"));
    json_free((json_value_t *)v);

    v = json_parse("{\"num\":123,\"str\":\"abc\",\"bool\":false,\"nul\":null}");
    assert_non_null(v);
    assert_non_null(strstr(json_text((const json_value_t *)v), "num"));
    assert_non_null(strstr(json_text((const json_value_t *)v), "str"));
    assert_non_null(strstr(json_text((const json_value_t *)v), "bool"));
    assert_non_null(strstr(json_text((const json_value_t *)v), "nul"));
    json_free((json_value_t *)v);
}

void test_parse_object_empty(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{}");
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_OBJECT);
    assert_int_equal((int)json_object_size(v), 0);
    json_free(v);
}

void test_parse_object_single_pair(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"a\":1}");
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_OBJECT);
    assert_int_equal((int)json_object_size(v), 1);

    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "a");

    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    double d;
    assert_true(json_value_get_number(val, &d));
    assert_true(d == 1.0);
    json_value_free_wrapper(val);

    json_free(v);
}

void test_parse_object_multiple_pairs_and_keys(void **state)
{
    (void)state;
    json_value_t *v = json_parse("{\"a\":1,\"b\":2}");
    assert_non_null(v);
    assert_int_equal((int)json_object_size(v), 2);
    const char *k0 = json_object_key(v, 0);
    const char *k1 = json_object_key(v, 1);
    /* keys may be in insertion order; assert both present */
    assert_true((strcmp(k0, "a") == 0 && strcmp(k1, "b") == 0) || (strcmp(k0, "b") == 0 && strcmp(k1, "a") == 0));

    json_free(v);
}

void test_parse_pair_key_value_parsing(void **state)
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

void test_parse_object_unexpected_delimiter(void **state)
{
    (void)state;
    /* legacy spec expects a SyntaxError for `{"a":1,}` */
    json_value_t *v = json_parse("{\"a\":1,}");
    assert_null(v);
}
