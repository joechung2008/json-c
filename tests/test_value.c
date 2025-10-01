#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

Test(value, parse_simple_value)
{
    const char   *txt = "{\"ok\":true}";
    json_value_t *v   = json_parse(txt);
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_OBJECT);
    cr_assert_eq((int)json_object_size(v), 1);
    const char *k = json_object_key(v, 0);
    cr_assert_str_eq(k, "ok");
    json_value_t *val = json_object_value(v, 0);
    cr_assert_not_null(val);
    cr_assert_eq(json_value_type(val), JSON_TRUE);
    bool b = false;
    cr_assert(json_value_get_bool(val, &b));
    cr_assert(b == true);
    json_value_free_wrapper(val);
    json_free(v);
}

Test(value, parse_value_booleans_null)
{
    void *v;

    v = json_parse("false");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("true");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("null");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(value, parse_value_number_and_string)
{
    void *v;

    v = json_parse("123");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("-45.67");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("\"hello world\"");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(value, parse_value_array_object_whitespace)
{
    void *v;

    v = json_parse("[1,2,3]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("{\"a\":1,\"b\":false}");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("   true   ");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(value, parse_array_and_values)
{
    json_value_t *v = json_parse("[1,2,3]");
    cr_assert_not_null(v);
    json_type_t t = json_value_type(v);
    cr_assert_eq(t, JSON_ARRAY);
    size_t n = json_array_length(v);
    cr_assert_eq((int)n, 3);

    json_value_t *e0 = json_array_get(v, 0);
    cr_assert_not_null(e0);
    double val = 0;
    cr_assert(json_value_get_number(e0, &val));
    cr_assert(val == 1.0);
    json_value_free_wrapper(e0);

    json_free(v);
}

Test(value, parse_value_array_type)
{
    json_value_t *v = json_parse("[1,2,3]");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_ARRAY);
    json_free(v);
}

Test(value, parse_value_array_values)
{
    json_value_t *v = json_parse("[1,2,3]");
    cr_assert_not_null(v);
    size_t n = json_array_length(v);
    cr_assert_eq((int)n, 3);

    json_value_t *e0 = json_array_get(v, 0);
    cr_assert_not_null(e0);
    double val = 0;
    cr_assert(json_value_get_number(e0, &val));
    cr_assert(val == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(v, 1);
    cr_assert_not_null(e1);
    cr_assert(json_value_get_number(e1, &val));
    cr_assert(val == 2.0);
    json_value_free_wrapper(e1);

    json_value_t *e2 = json_array_get(v, 2);
    cr_assert_not_null(e2);
    cr_assert(json_value_get_number(e2, &val));
    cr_assert(val == 3.0);
    json_value_free_wrapper(e2);

    json_free(v);
}

Test(value, parse_invalid_input)
{
    json_value_t *v = json_parse("invalid");
    cr_assert_null(v);
}

Test(value, parse_empty_input)
{
    json_value_t *v = json_parse("");
    cr_assert_null(v);
}

Test(value, parse_malformed_false_literal)
{
    json_value_t *v = json_parse("fals");
    cr_assert_null(v);
}

Test(value, parse_malformed_true_literal)
{
    json_value_t *v = json_parse("tru");
    cr_assert_null(v);
}

Test(value, parse_malformed_null_literal)
{
    json_value_t *v = json_parse("nul");
    cr_assert_null(v);
}

Test(value, parse_unexpected_character)
{
    json_value_t *v = json_parse("@");
    cr_assert_null(v);
}
