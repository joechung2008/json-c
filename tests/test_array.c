#include <json_c/json.h>

#include <stdarg.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

void test_parse_simple_array(void **state)
{
    (void)state;
    const char   *txt = "[1, 2, 3]";
    json_value_t *v   = json_parse(txt);
    assert_non_null(v);
    assert_string_equal(json_text(v), txt);
    json_free(v);
}

void test_parse_array_empty_and_numbers(void **state)
{
    (void)state;
    void *v;

    v = json_parse("[]");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[1,2,3]");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_array_of_strings_and_booleans(void **state)
{
    (void)state;
    void *v;

    v = json_parse("[\"a\",\"b\",\"c\"]");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[true,false,true]");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_array_nested_and_mixed(void **state)
{
    (void)state;
    void *v;

    v = json_parse("[{\"a\":1},{\"b\":2}]");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[[1],[2,3]]");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[1,\"a\",true,null,{\"x\":2},[3,4]]");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_array_empty_type(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[]");
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_ARRAY);
    assert_int_equal((int)json_array_length(v), 0);
    json_free(v);
}

void test_parse_array_mixed_types(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[1,\"a\",true,null]");
    assert_non_null(v);
    assert_int_equal((int)json_array_length(v), 4);

    json_value_t *e0 = json_array_get(v, 0);
    double        d;
    assert_true(json_value_get_number(e0, &d));
    assert_true(d == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(v, 1);
    const char   *s  = json_value_get_string(e1);
    assert_string_equal(s, "a");
    json_value_free_wrapper(e1);

    json_value_t *e2 = json_array_get(v, 2);
    int           b;
    assert_true(json_value_get_bool(e2, &b));
    assert_int_equal(b, 1);
    json_value_free_wrapper(e2);

    json_value_t *e3 = json_array_get(v, 3);
    int           dummy;
    assert_false(json_value_get_bool(e3, &dummy)); /* null */
    json_value_free_wrapper(e3);

    json_free(v);
}

void test_parse_array_nested_first_element(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[[1,2],3]");
    assert_non_null(v);

    /* first element should be an array */
    json_value_t *nested = json_array_get(v, 0);
    assert_non_null(nested);
    assert_int_equal(json_value_type(nested), JSON_ARRAY);

    /* nested array should contain 1 and 2 */
    size_t n = json_array_length(nested);
    assert_int_equal((int)n, 2);

    json_value_t *e0 = json_array_get(nested, 0);
    double        d;
    assert_true(json_value_get_number(e0, &d));
    assert_true(d == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(nested, 1);
    assert_true(json_value_get_number(e1, &d));
    assert_true(d == 2.0);
    json_value_free_wrapper(e1);

    json_value_free_wrapper(nested);
    json_free(v);
}

void test_parse_array_nested_second_element(void **state)
{
    (void)state;
    json_value_t *v = json_parse("[[1,2],3]");
    assert_non_null(v);

    json_value_t *e1 = json_array_get(v, 1);
    assert_non_null(e1);
    double d;
    assert_true(json_value_get_number(e1, &d));
    assert_true(d == 3.0);
    json_value_free_wrapper(e1);

    json_free(v);
}

void test_parse_array_missing_opening_bracket(void **state)
{
    (void)state;
    /* malformed: missing opening bracket */
    json_value_t *v = json_parse("1,2,3]");
    assert_null(v);
}

void test_parse_array_unexpected_delimiter(void **state)
{
    (void)state;
    /* malformed: trailing comma before closing bracket */
    json_value_t *v = json_parse("[1,2,3,]");
    assert_null(v);
}
