#include <json_c/json.h>

#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

/* Test parsing of simple numbers */
Test(number, test_parse_number_simple)
{
    const char   *input = "12345";
    json_value_t *v     = json_parse(input);
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_NUMBER);
    double d;
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == 12345.0);
    json_free(v);
}

Test(number, test_parse_number_positive_and_negative)
{
    void *v;

    v = json_parse("42");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);

    v = json_parse("-7");
    cr_assert_not_null(v);
    json_free((json_value_t *)v);
}

Test(number, test_parse_number_integer_and_types)
{
    /* Check a few integer examples, including 123 from earlier variant */
    json_value_t *v = json_parse("123");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_NUMBER);
    double d;
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == 123.0);
    json_free(v);

    v = json_parse("42");
    cr_assert_not_null(v);
    cr_assert_eq(json_value_type(v), JSON_NUMBER);
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == 42.0);
    json_free(v);

    v = json_parse("-7");
    cr_assert_not_null(v);
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == -7.0);
    json_free(v);
}

Test(number, test_parse_number_decimal_and_exponent)
{
    json_value_t *v = json_parse("3.14");
    cr_assert_not_null(v);
    double d;
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == 3.14);
    json_free(v);

    v = json_parse("-2.71");
    cr_assert_not_null(v);
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == -2.71);
    json_free(v);

    v = json_parse("6.02e23");
    cr_assert_not_null(v);
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == 6.02e23);
    json_free(v);

    v = json_parse("1.23e-4");
    cr_assert_not_null(v);
    cr_assert(json_value_get_number(v, &d));
    cr_assert(d == 1.23e-4);
    json_free(v);
}

Test(number, test_parse_number_invalid_cases)
{
    json_value_t *v = json_parse("-");
    cr_assert_null(v);
    v = json_parse("abc");
    cr_assert_null(v);
}
