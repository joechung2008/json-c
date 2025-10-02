#include <json_c/json.h>

#include <setjmp.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

static void test_parse_simple(void **state)
{
    (void)state;
    const char   *text = "{\"ok\":true}";
    json_value_t *v    = json_parse(text);
    assert_non_null(v);
    assert_int_equal(json_value_type(v), JSON_OBJECT);
    assert_int_equal((int)json_object_size(v), 1);
    const char *k = json_object_key(v, 0);
    assert_string_equal(k, "ok");
    json_value_t *val = json_object_value(v, 0);
    assert_non_null(val);
    assert_int_equal(json_value_type(val), JSON_TRUE);
    bool b = false;
    assert_true(json_value_get_bool(val, &b));
    assert_true(b == true);
    json_value_free_wrapper(val);
    json_free(v);
}

/* Forward-declared test functions from other compilation units */
void test_parse_array_and_values(void **state);
void test_parse_array_empty_and_numbers(void **state);
void test_parse_array_empty_type(void **state);
void test_parse_array_missing_opening_bracket(void **state);
void test_parse_array_mixed_types(void **state);
void test_parse_array_nested_and_mixed(void **state);
void test_parse_array_nested_first_element(void **state);
void test_parse_array_nested_second_element(void **state);
void test_parse_array_of_strings_and_booleans(void **state);
void test_parse_array_unexpected_delimiter(void **state);
void test_parse_empty_input(void **state);
void test_parse_empty_object(void **state);
void test_parse_invalid_input(void **state);
void test_parse_malformed_false_literal(void **state);
void test_parse_malformed_null_literal(void **state);
void test_parse_malformed_true_literal(void **state);
void test_parse_multiple_pairs_and_commas(void **state);
void test_parse_number_decimal_and_exponent(void **state);
void test_parse_number_integer_and_types(void **state);
void test_parse_number_invalid_cases(void **state);
void test_parse_number_positive_and_negative(void **state);
void test_parse_number_simple(void **state);
void test_parse_object_empty(void **state);
void test_parse_object_multiple_pairs_and_keys(void **state);
void test_parse_object_nested_and_mixed(void **state);
void test_parse_object_single_and_multiple(void **state);
void test_parse_object_single_pair(void **state);
void test_parse_object_unexpected_delimiter(void **state);
void test_parse_pair_errors(void **state);
void test_parse_pair_key_and_value_bool(void **state);
void test_parse_pair_key_and_value_null(void **state);
void test_parse_pair_key_and_value_number(void **state);
void test_parse_pair_key_and_value_string(void **state);
void test_parse_pair_key_value_parsing(void **state);
void test_parse_pair_simple(void **state);
void test_parse_pair_value_array_and_object_types(void **state);
void test_parse_pair_whitespace_handling(void **state);
void test_parse_simple_array(void **state);
void test_parse_simple_value(void **state);
void test_parse_string_empty(void **state);
void test_parse_string_escapes_backslash_and_slash(void **state);
void test_parse_string_escapes_quotes(void **state);
void test_parse_string_escapes(void **state);
void test_parse_string_invalid_escape(void **state);
void test_parse_string_invalid_unicode(void **state);
void test_parse_string_malformed_escape_sequences(void **state);
void test_parse_string_missing_opening_quote(void **state);
void test_parse_string_simple(void **state);
void test_parse_string_unicode(void **state);
void test_parse_string_unterminated(void **state);
void test_parse_unexpected_character(void **state);
void test_parse_value_array_object_whitespace(void **state);
void test_parse_value_array_type(void **state);
void test_parse_value_array_values(void **state);
void test_parse_value_booleans_null(void **state);
void test_parse_value_number_and_string(void **state);
void test_print_array_token(void **state);
void test_print_array_with_array_element(void **state);
void test_print_array_with_object_element(void **state);
void test_print_array_with_nested_nonempty_array(void **state);
void test_print_array_with_nested_nonempty_object(void **state);
void test_print_false_token(void **state);
void test_print_null_token(void **state);
void test_print_number_token(void **state);
void test_print_object_token(void **state);
void test_print_object_with_pair_array_value(void **state);
void test_print_object_with_pair_object_value(void **state);
void test_print_object_with_nested_nonempty_array(void **state);
void test_print_object_with_nested_nonempty_object(void **state);
void test_print_pair_token(void **state);
void test_print_string_token(void **state);
void test_print_true_token(void **state);

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_array_and_values),
        cmocka_unit_test(test_parse_array_empty_and_numbers),
        cmocka_unit_test(test_parse_array_empty_type),
        cmocka_unit_test(test_parse_array_missing_opening_bracket),
        cmocka_unit_test(test_parse_array_mixed_types),
        cmocka_unit_test(test_parse_array_nested_and_mixed),
        cmocka_unit_test(test_parse_array_nested_first_element),
        cmocka_unit_test(test_parse_array_nested_second_element),
        cmocka_unit_test(test_parse_array_of_strings_and_booleans),
        cmocka_unit_test(test_parse_array_unexpected_delimiter),
        cmocka_unit_test(test_parse_empty_input),
        cmocka_unit_test(test_parse_empty_object),
        cmocka_unit_test(test_parse_invalid_input),
        cmocka_unit_test(test_parse_malformed_false_literal),
        cmocka_unit_test(test_parse_malformed_null_literal),
        cmocka_unit_test(test_parse_malformed_true_literal),
        cmocka_unit_test(test_parse_multiple_pairs_and_commas),
        cmocka_unit_test(test_parse_number_decimal_and_exponent),
        cmocka_unit_test(test_parse_number_integer_and_types),
        cmocka_unit_test(test_parse_number_invalid_cases),
        cmocka_unit_test(test_parse_number_positive_and_negative),
        cmocka_unit_test(test_parse_number_simple),
        cmocka_unit_test(test_parse_object_empty),
        cmocka_unit_test(test_parse_object_multiple_pairs_and_keys),
        cmocka_unit_test(test_parse_object_nested_and_mixed),
        cmocka_unit_test(test_parse_object_single_and_multiple),
        cmocka_unit_test(test_parse_object_single_pair),
        cmocka_unit_test(test_parse_object_unexpected_delimiter),
        cmocka_unit_test(test_parse_pair_errors),
        cmocka_unit_test(test_parse_pair_key_and_value_bool),
        cmocka_unit_test(test_parse_pair_key_and_value_null),
        cmocka_unit_test(test_parse_pair_key_and_value_number),
        cmocka_unit_test(test_parse_pair_key_and_value_string),
        cmocka_unit_test(test_parse_pair_key_value_parsing),
        cmocka_unit_test(test_parse_pair_simple),
        cmocka_unit_test(test_parse_pair_value_array_and_object_types),
        cmocka_unit_test(test_parse_pair_whitespace_handling),
        cmocka_unit_test(test_parse_simple),
        cmocka_unit_test(test_parse_simple_array),
        cmocka_unit_test(test_parse_simple_value),
        cmocka_unit_test(test_parse_string_empty),
        cmocka_unit_test(test_parse_string_escapes),
        cmocka_unit_test(test_parse_string_escapes_backslash_and_slash),
        cmocka_unit_test(test_parse_string_escapes_quotes),
        cmocka_unit_test(test_parse_string_invalid_escape),
        cmocka_unit_test(test_parse_string_invalid_unicode),
        cmocka_unit_test(test_parse_string_malformed_escape_sequences),
        cmocka_unit_test(test_parse_string_missing_opening_quote),
        cmocka_unit_test(test_parse_string_simple),
        cmocka_unit_test(test_parse_string_unicode),
        cmocka_unit_test(test_parse_string_unterminated),
        cmocka_unit_test(test_parse_unexpected_character),
        cmocka_unit_test(test_parse_value_array_object_whitespace),
        cmocka_unit_test(test_parse_value_array_type),
        cmocka_unit_test(test_parse_value_array_values),
        cmocka_unit_test(test_parse_value_booleans_null),
        cmocka_unit_test(test_parse_value_number_and_string),
        cmocka_unit_test(test_print_array_token),
        cmocka_unit_test(test_print_array_with_array_element),
        cmocka_unit_test(test_print_array_with_object_element),
        cmocka_unit_test(test_print_array_with_nested_nonempty_array),
        cmocka_unit_test(test_print_array_with_nested_nonempty_object),
        cmocka_unit_test(test_print_false_token),
        cmocka_unit_test(test_print_null_token),
        cmocka_unit_test(test_print_number_token),
        cmocka_unit_test(test_print_object_token),
        cmocka_unit_test(test_print_object_with_pair_array_value),
        cmocka_unit_test(test_print_object_with_pair_object_value),
        cmocka_unit_test(test_print_object_with_nested_nonempty_array),
        cmocka_unit_test(test_print_object_with_nested_nonempty_object),
        cmocka_unit_test(test_print_pair_token),
        cmocka_unit_test(test_print_string_token),
        cmocka_unit_test(test_print_true_token),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
