#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

Test(object, test_parse_empty_object)
{
    json_value_t *v = json_parse("{}");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_OBJECT);
    cr_assert_eq((int)json_object_size(v), 0);
    json_free(v);
}

Test(object, test_parse_object_single_and_multiple)
{
    void *v;

    v = json_parse("{\"a\":1}");
    cr_assert_not_null(v);
    /* textual checks for key presence */
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "\"a\""));
    json_free((json_value_t *)v);

    v = json_parse("{\"x\":true,\"y\":null,\"z\":\"abc\"}");
    cr_assert_not_null(v);
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "\"x\""));
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "\"y\""));
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "\"z\""));
    json_free((json_value_t *)v);
}

Test(object, test_parse_object_nested_and_mixed)
{
    void *v;

    v = json_parse("{\"outer\":{\"inner\":42}}");
    cr_assert_not_null(v);
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "outer"));
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "inner"));
    json_free((json_value_t *)v);

    v = json_parse("{\"arr\":[1,2,3]}");
    cr_assert_not_null(v);
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "arr"));
    json_free((json_value_t *)v);

    v = json_parse("{\"num\":123,\"str\":\"abc\",\"bool\":false,\"nul\":null}");
    cr_assert_not_null(v);
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "num"));
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "str"));
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "bool"));
    cr_assert_not_null(strstr(json_text((const json_value_t *)v), "nul"));
    json_free((json_value_t *)v);
}

Test(object, test_parse_object_empty)
{
    json_value_t *v = json_parse("{}");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_OBJECT);
    cr_assert_eq((int)json_object_size(v), 0);
    json_free(v);
}

Test(object, test_parse_object_single_pair)
{
    json_value_t *v = json_parse("{\"a\":1}");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_OBJECT);
    cr_assert_eq((int)json_object_size(v), 1);

    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "a");

    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    double d;
    cr_assert(json_value_get_number(val, &d));
    cr_assert(d == 1.0);
    json_value_free_wrapper(val);

    json_free(v);
}

Test(object, test_parse_object_multiple_pairs_and_keys)
{
    json_value_t *v = json_parse("{\"a\":1,\"b\":2}");
    cr_assert_not_null(v);
    cr_assert_eq((int)json_object_size(v), 2);
    const char *k0 = json_object_key(v, 0);
    const char *k1 = json_object_key(v, 1);
    /* keys may be in insertion order; assert both present */
    cr_assert((strcmp(k0, "a") == 0 && strcmp(k1, "b") == 0) || (strcmp(k0, "b") == 0 && strcmp(k1, "a") == 0));

    json_free(v);
}

Test(object, test_parse_pair_key_value_parsing)
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

Test(object, test_parse_object_unexpected_delimiter)
{
    /* legacy spec expects a SyntaxError for `{"a":1,}` */
    json_value_t *v = json_parse("{\"a\":1,}");
    cr_assert_null(v);
}
