#include <json_c/json.h>

#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

/* Test parsing of simple numbers */
void test_parse_number_simple(void **state)
{
    (void)state;
    const char   *input = "12345";
    json_value_t *v     = json_parse(input);
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_NUMBER);
    double d;
    assert_true(json_value_get_number(v, &d));
    assert_true(d == 12345.0);
    json_free(v);
}

void test_parse_number_positive_and_negative(void **state)
{
    (void)state;
    void *v;

    v = json_parse("42");
    assert_non_null(v);
    json_free((json_value_t *)v);

    v = json_parse("-7");
    assert_non_null(v);
    json_free((json_value_t *)v);
}

void test_parse_number_integer_and_types(void **state)
{
    (void)state;
    /* Check a few integer examples, including 123 from earlier variant */
    json_value_t *v = json_parse("123");
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_NUMBER);
    double d;
    assert_true(json_value_get_number(v, &d));
    assert_true(d == 123.0);
    json_free(v);

    v = json_parse("42");
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_NUMBER);
    assert_true(json_value_get_number(v, &d));
    assert_true(d == 42.0);
    json_free(v);

    v = json_parse("-7");
    assert_non_null(v);
    assert_true(json_value_get_number(v, &d));
    assert_true(d == -7.0);
    json_free(v);
}

void test_parse_number_decimal_and_exponent(void **state)
{
    (void)state;
    json_value_t *v = json_parse("3.14");
    assert_non_null(v);
    double d;
    assert_true(json_value_get_number(v, &d));
    assert_true(d == 3.14);
    json_free(v);

    v = json_parse("-2.71");
    assert_non_null(v);
    assert_true(json_value_get_number(v, &d));
    assert_true(d == -2.71);
    json_free(v);

    v = json_parse("6.02e23");
    assert_non_null(v);
    assert_true(json_value_get_number(v, &d));
    assert_true(d == 6.02e23);
    json_free(v);

    v = json_parse("1.23e-4");
    assert_non_null(v);
    assert_true(json_value_get_number(v, &d));
    assert_true(d == 1.23e-4);
    json_free(v);
}

void test_parse_number_invalid_cases(void **state)
{
    (void)state;
    json_value_t *v = json_parse("-");
    assert_null(v);
    v = json_parse("abc");
    assert_null(v);
}
