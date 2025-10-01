#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

void test_parse_string_simple(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"hello\"");
    assert_non_null(v);
    const char *s = json_value_get_string(v);
    assert_string_equal(s, "hello");
    json_free(v);
}

void test_parse_string_empty(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"\"");
    assert_non_null(v);
    json_free(v);
}

void test_parse_string_escapes_quotes(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"foo \\\"bar\\\" baz\"");
    assert_non_null(v);
    json_free(v);
}

void test_parse_string_unicode(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"\\u0041\\u0042\\u0043\"");
    assert_non_null(v);
    json_free(v);
}

void test_parse_string_escapes(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"he\\\"llo\"");
    assert_non_null(v);
    const char *s = json_value_get_string(v);
    assert_string_equal(s, "he\"llo");
    json_free(v);
}

void test_parse_string_escapes_backslash_and_slash(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"he\\\\llo\"");
    assert_non_null(v);
    const char *s = json_value_get_string(v);
    assert_string_equal(s, "he\\llo");
    json_free(v);

    v = json_parse("\"he\\/llo\"");
    assert_non_null(v);
    s = json_value_get_string(v);
    assert_string_equal(s, "he/llo");
    json_free(v);
}

void test_parse_string_invalid_escape(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"hello\\x\"");
    assert_null(v);
}

void test_parse_string_invalid_unicode(void **state)
{
    (void)state;
    json_value_t *v = json_parse("\"\\uXYZ\"");
    assert_null(v);

    v = json_parse("\"\\u123\"");
    assert_null(v);
}

void test_parse_string_missing_opening_quote(void **state)
{
    (void)state;
    /* legacy spec: "hello" without opening quote should be a syntax error */
    json_value_t *v = json_parse("hello\"");
    assert_null(v);
}

void test_parse_string_unterminated(void **state)
{
    (void)state;
    /* Unterminated string should be a parse error */
    json_value_t *v = json_parse("\"unterminated");
    assert_null(v);
    v = json_parse("{\"a\":\"b");
    assert_null(v);
}

void test_parse_string_malformed_escape_sequences(void **state)
{
    (void)state;
    /* Backslash followed by nothing or an invalid escape should fail */
    json_value_t *v = json_parse("\"bad\\\"");
    assert_null(v);
    v = json_parse("\"bad\\z\"");
    assert_null(v);
}
