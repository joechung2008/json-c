#include <json_c/json.h>

#include <stdarg.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

Test(array, test_parse_simple_array)
{
    const char   *txt = "[1, 2, 3]";
    json_value_t *v   = json_parse(txt);
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_ARRAY);
    cr_assert_eq((int)json_array_length(v), 3);

    double        d;
    json_value_t *e0 = json_array_get(v, 0);
    cr_assert(json_value_get_number(e0, &d));
    cr_assert(d == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(v, 1);
    cr_assert(json_value_get_number(e1, &d));
    cr_assert(d == 2.0);
    json_value_free_wrapper(e1);

    json_value_t *e2 = json_array_get(v, 2);
    cr_assert(json_value_get_number(e2, &d));
    cr_assert(d == 3.0);
    json_value_free_wrapper(e2);

    json_free(v);
}

Test(array, test_parse_array_empty_and_numbers)
{
    void *v;

    v = json_parse("[]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[1,2,3]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(array, test_parse_array_of_strings_and_booleans)
{
    void *v;

    v = json_parse("[\"a\",\"b\",\"c\"]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[true,false,true]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(array, test_parse_array_nested_and_mixed)
{
    void *v;

    v = json_parse("[{\"a\":1},{\"b\":2}]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[[1],[2,3]]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("[1,\"a\",true,null,{\"x\":2},[3,4]]");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(array, test_parse_array_empty_type)
{
    json_value_t *v = json_parse("[]");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_ARRAY);
    cr_assert_eq((int)json_array_length(v), 0);
    json_free(v);
}

Test(array, test_parse_array_mixed_types)
{
    json_value_t *v = json_parse("[1,\"a\",true,null]");
    cr_assert_not_null(v);
    cr_assert_eq((int)json_array_length(v), 4);

    json_value_t *e0 = json_array_get(v, 0);
    double        d;
    cr_assert(json_value_get_number(e0, &d));
    cr_assert(d == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(v, 1);
    const char   *s  = json_value_get_string(e1);
    cr_assert_str_eq(s, "a");
    json_value_free_wrapper(e1);

    json_value_t *e2 = json_array_get(v, 2);
    bool          b;
    cr_assert(json_value_get_bool(e2, &b));
    cr_assert(b);
    json_value_free_wrapper(e2);

    json_value_t *e3 = json_array_get(v, 3);
    bool          dummy;
    cr_assert_not(json_value_get_bool(e3, &dummy)); /* null */
    json_value_free_wrapper(e3);

    json_free(v);
}

Test(array, test_parse_array_nested_first_element)
{
    json_value_t *v = json_parse("[[1,2],3]");
    cr_assert_not_null(v);

    /* first element should be an array */
    json_value_t *nested = json_array_get(v, 0);
    cr_assert_not_null(nested);
    cr_assert_eq(json_value_type(nested), JSON_ARRAY);

    /* nested array should contain 1 and 2 */
    size_t n = json_array_length(nested);
    cr_assert_eq((int)n, 2);

    json_value_t *e0 = json_array_get(nested, 0);
    double        d;
    cr_assert(json_value_get_number(e0, &d));
    cr_assert(d == 1.0);
    json_value_free_wrapper(e0);

    json_value_t *e1 = json_array_get(nested, 1);
    cr_assert(json_value_get_number(e1, &d));
    cr_assert(d == 2.0);
    json_value_free_wrapper(e1);

    json_value_free_wrapper(nested);
    json_free(v);
}

Test(array, test_parse_array_nested_second_element)
{
    json_value_t *v = json_parse("[[1,2],3]");
    cr_assert_not_null(v);

    json_value_t *e1 = json_array_get(v, 1);
    cr_assert_not_null(e1);
    double d;
    cr_assert(json_value_get_number(e1, &d));
    cr_assert(d == 3.0);
    json_value_free_wrapper(e1);

    json_free(v);
}

Test(array, test_parse_array_missing_opening_bracket)
{
    /* malformed: missing opening bracket */
    json_value_t *v = json_parse("1,2,3]");
    cr_assert_null(v);
}

Test(array, test_parse_array_unexpected_delimiter)
{
    /* malformed: trailing comma before closing bracket */
    json_value_t *v = json_parse("[1,2,3,]");
    cr_assert_null(v);
}
