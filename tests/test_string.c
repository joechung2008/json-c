#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>

Test(string, test_parse_string_simple)
{
    json_value_t *v = json_parse("\"hello\"");
    cr_assert_not_null(v);
    const char *s = json_value_get_string(v);
    cr_assert_str_eq(s, "hello");
    json_free(v);
}

Test(string, test_parse_string_empty)
{
    json_value_t *v = json_parse("\"\"");
    cr_assert_not_null(v);
    json_free(v);
}

Test(string, test_parse_string_escapes_quotes)
{
    json_value_t *v = json_parse("\"foo \\\"bar\\\" baz\"");
    cr_assert_not_null(v);
    json_free(v);
}

Test(string, test_parse_string_unicode)
{
    json_value_t *v = json_parse("\"\\u0041\\u0042\\u0043\"");
    cr_assert_not_null(v);
    json_free(v);
}

Test(string, test_parse_string_escapes)
{
    json_value_t *v = json_parse("\"he\\\"llo\"");
    cr_assert_not_null(v);
    const char *s = json_value_get_string(v);
    cr_assert_str_eq(s, "he\"llo");
    json_free(v);
}

Test(string, test_parse_string_escapes_backslash_and_slash)
{
    json_value_t *v = json_parse("\"he\\\\llo\"");
    cr_assert_not_null(v);
    const char *s = json_value_get_string(v);
    cr_assert_str_eq(s, "he\\llo");
    json_free(v);

    v = json_parse("\"he\\/llo\"");
    cr_assert_not_null(v);
    s = json_value_get_string(v);
    cr_assert_str_eq(s, "he/llo");
    json_free(v);
}

Test(string, test_parse_string_invalid_escape)
{
    json_value_t *v = json_parse("\"hello\\x\"");
    cr_assert_null(v);
}

Test(string, test_parse_string_invalid_unicode)
{
    json_value_t *v = json_parse("\"\\uXYZ\"");
    cr_assert_null(v);

    v = json_parse("\"\\u123\"");
    cr_assert_null(v);
}

Test(string, test_parse_string_missing_opening_quote)
{
    /* legacy spec: "hello" without opening quote should be a syntax error */
    json_value_t *v = json_parse("hello\"");
    cr_assert_null(v);
}

Test(string, test_parse_string_unterminated)
{
    /* Unterminated string should be a parse error */
    json_value_t *v = json_parse("\"unterminated");
    cr_assert_null(v);
    v = json_parse("{\"a\":\"b");
    cr_assert_null(v);
}

Test(string, test_parse_string_malformed_escape_sequences)
{
    /* Backslash followed by nothing or an invalid escape should fail */
    json_value_t *v = json_parse("\"bad\\\"");
    cr_assert_null(v);
    v = json_parse("\"bad\\z\"");
    cr_assert_null(v);
}
