#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

/* Test parsing of a simple pair inside an object */
Test(pair, test_parse_pair_simple)
{
    const char *input = "{\"a\": 1}";
    void       *v     = json_parse(input);
    cr_assert_not_null(v);
    const char *txt = json_text((const json_value_t *)v);
    cr_assert_not_null(txt);
    /* we expect the textual representation to contain the key "a" */
    cr_assert_not_null(strstr(txt, "\"a\""));
    json_free((json_value_t *)v);
}

Test(pair, test_parse_pair_key_and_value_string)
{
    json_value_t *v = json_parse("{\"name\": \"John\"}");
    cr_assert_not_null(v);

    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "name");

    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    const char *s = json_value_get_string(val);
    cr_assert_str_eq(s, "John");
    json_value_free_wrapper(val);

    json_free(v);
}

Test(pair, test_parse_pair_key_and_value_number)
{
    json_value_t *v = json_parse("{\"age\": 42}");
    cr_assert_not_null(v);

    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "age");

    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    double d = 0;
    cr_assert(json_value_get_number(val, &d));
    cr_assert(d == 42.0);
    json_value_free_wrapper(val);

    json_free(v);
}

Test(pair, test_parse_pair_key_and_value_bool)
{
    json_value_t *v = json_parse("{\"active\": true}");
    cr_assert_not_null(v);

    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "active");

    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    bool b = false;
    cr_assert(json_value_get_bool(val, &b));
    cr_assert(b);
    json_value_free_wrapper(val);

    json_free(v);
}

Test(pair, test_parse_pair_key_and_value_null)
{
    json_value_t *v = json_parse("{\"data\": null}");
    cr_assert_not_null(v);

    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "data");

    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    cr_assert_eq(json_value_type(val), JSON_NULL);
    json_value_free_wrapper(val);

    json_free(v);
}

Test(pair, test_parse_pair_value_array_and_object_types)
{
    json_value_t *v = json_parse("{\"items\": [1,2,3]}");
    cr_assert_not_null(v);
    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "items");
    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    cr_assert_eq(json_value_type(val), JSON_ARRAY);
    json_value_free_wrapper(val);
    json_free(v);

    v = json_parse("{\"nested\": {\"inner\": \"value\"}}");
    cr_assert_not_null(v);
    k = json_object_key(v, 0);
    cr_assert_str_eq(k, "nested");
    val = json_object_value(v, 0);
    cr_assert_not_null(val);
    cr_assert_eq(json_value_type(val), JSON_OBJECT);
    json_value_free_wrapper(val);
    json_free(v);
}

Test(pair, test_parse_pair_whitespace_handling)
{
    json_value_t *v = json_parse("{\"key\"  :  \"value\"}");
    cr_assert_not_null(v);
    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "key");
    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    const char *s = json_value_get_string(val);
    cr_assert_str_eq(s, "value");
    json_value_free_wrapper(val);
    json_free(v);

    v = json_parse("{  \"key\": \"value\"  }");
    cr_assert_not_null(v);
    k = json_object_key(v, 0);
    cr_assert_str_eq(k, "key");
    val = json_object_value(v, 0);
    cr_assert_not_null(val);
    s = json_value_get_string(val);
    cr_assert_str_eq(s, "value");
    json_value_free_wrapper(val);
    json_free(v);
}

Test(pair, test_parse_pair_errors)
{
    json_value_t *v;

    v = json_parse("{\"key\" \"value\"}");
    cr_assert_null(v);

    v = json_parse("{\"key\"; \"value\"}");
    cr_assert_null(v);

    v = json_parse("{key\": \"value\"}");
    cr_assert_null(v);

    v = json_parse("{\"key\": invalid}");
    cr_assert_null(v);
}

Test(pair, test_parse_multiple_pairs_and_commas)
{
    json_value_t *v = json_parse("{\"key1\": \"value1\", \"key2\": \"value2\"}");
    cr_assert_not_null(v);

    const char *k0 = json_object_key(v, 0);
    cr_assert_str_eq(k0, "key1");
    json_value_t *val0 = json_object_value(v, 0);
    cr_assert_not_null(val0);
    const char *s0 = json_value_get_string(val0);
    cr_assert_str_eq(s0, "value1");
    json_value_free_wrapper(val0);

    const char *k1 = json_object_key(v, 1);
    cr_assert_str_eq(k1, "key2");
    json_value_t *val1 = json_object_value(v, 1);
    cr_assert_not_null(val1);
    const char *s1 = json_value_get_string(val1);
    cr_assert_str_eq(s1, "value2");
    json_value_free_wrapper(val1);

    json_free(v);
}
