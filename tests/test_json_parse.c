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

/* Forward-declared test functions from other compilation units (unique) */
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
void test_print_false_token(void **state);
void test_print_null_token(void **state);
void test_print_number_token(void **state);
void test_print_object_token(void **state);
void test_print_object_with_pair_array_value(void **state);
void test_print_object_with_pair_object_value(void **state);
void test_print_pair_token(void **state);
void test_print_string_token(void **state);
void test_print_true_token(void **state);

typedef struct
{
    const char        *name;
    CMUnitTestFunction func;
} test_entry_t;

static test_entry_t all_tests[] = {
    {.name = "test_parse_array_and_values", .func = test_parse_array_and_values},
    {.name = "test_parse_array_empty_and_numbers", .func = test_parse_array_empty_and_numbers},
    {.name = "test_parse_array_empty_type", .func = test_parse_array_empty_type},
    {.name = "test_parse_array_missing_opening_bracket", .func = test_parse_array_missing_opening_bracket},
    {.name = "test_parse_array_mixed_types", .func = test_parse_array_mixed_types},
    {.name = "test_parse_array_nested_and_mixed", .func = test_parse_array_nested_and_mixed},
    {.name = "test_parse_array_nested_first_element", .func = test_parse_array_nested_first_element},
    {.name = "test_parse_array_nested_second_element", .func = test_parse_array_nested_second_element},
    {.name = "test_parse_array_of_strings_and_booleans", .func = test_parse_array_of_strings_and_booleans},
    {.name = "test_parse_array_unexpected_delimiter", .func = test_parse_array_unexpected_delimiter},
    {.name = "test_parse_empty_input", .func = test_parse_empty_input},
    {.name = "test_parse_empty_object", .func = test_parse_empty_object},
    {.name = "test_parse_invalid_input", .func = test_parse_invalid_input},
    {.name = "test_parse_malformed_false_literal", .func = test_parse_malformed_false_literal},
    {.name = "test_parse_malformed_null_literal", .func = test_parse_malformed_null_literal},
    {.name = "test_parse_malformed_true_literal", .func = test_parse_malformed_true_literal},
    {.name = "test_parse_multiple_pairs_and_commas", .func = test_parse_multiple_pairs_and_commas},
    {.name = "test_parse_number_decimal_and_exponent", .func = test_parse_number_decimal_and_exponent},
    {.name = "test_parse_number_integer_and_types", .func = test_parse_number_integer_and_types},
    {.name = "test_parse_number_invalid_cases", .func = test_parse_number_invalid_cases},
    {.name = "test_parse_number_positive_and_negative", .func = test_parse_number_positive_and_negative},
    {.name = "test_parse_number_simple", .func = test_parse_number_simple},
    {.name = "test_parse_object_empty", .func = test_parse_object_empty},
    {.name = "test_parse_object_multiple_pairs_and_keys", .func = test_parse_object_multiple_pairs_and_keys},
    {.name = "test_parse_object_nested_and_mixed", .func = test_parse_object_nested_and_mixed},
    {.name = "test_parse_object_single_and_multiple", .func = test_parse_object_single_and_multiple},
    {.name = "test_parse_object_single_pair", .func = test_parse_object_single_pair},
    {.name = "test_parse_object_unexpected_delimiter", .func = test_parse_object_unexpected_delimiter},
    {.name = "test_parse_pair_errors", .func = test_parse_pair_errors},
    {.name = "test_parse_pair_key_and_value_bool", .func = test_parse_pair_key_and_value_bool},
    {.name = "test_parse_pair_key_and_value_null", .func = test_parse_pair_key_and_value_null},
    {.name = "test_parse_pair_key_and_value_number", .func = test_parse_pair_key_and_value_number},
    {.name = "test_parse_pair_key_and_value_string", .func = test_parse_pair_key_and_value_string},
    {.name = "test_parse_pair_key_value_parsing", .func = test_parse_pair_key_value_parsing},
    {.name = "test_parse_pair_simple", .func = test_parse_pair_simple},
    {.name = "test_parse_pair_value_array_and_object_types", .func = test_parse_pair_value_array_and_object_types},
    {.name = "test_parse_pair_whitespace_handling", .func = test_parse_pair_whitespace_handling},
    {.name = "test_parse_simple_array", .func = test_parse_simple_array},
    {.name = "test_parse_simple_value", .func = test_parse_simple_value},
    {.name = "test_parse_simple", .func = test_parse_simple},
    {.name = "test_parse_string_empty", .func = test_parse_string_empty},
    {.name = "test_parse_string_escapes_backslash_and_slash", .func = test_parse_string_escapes_backslash_and_slash},
    {.name = "test_parse_string_escapes_quotes", .func = test_parse_string_escapes_quotes},
    {.name = "test_parse_string_escapes", .func = test_parse_string_escapes},
    {.name = "test_parse_string_invalid_escape", .func = test_parse_string_invalid_escape},
    {.name = "test_parse_string_invalid_unicode", .func = test_parse_string_invalid_unicode},
    {.name = "test_parse_string_malformed_escape_sequences", .func = test_parse_string_malformed_escape_sequences},
    {.name = "test_parse_string_missing_opening_quote", .func = test_parse_string_missing_opening_quote},
    {.name = "test_parse_string_simple", .func = test_parse_string_simple},
    {.name = "test_parse_string_unicode", .func = test_parse_string_unicode},
    {.name = "test_parse_string_unterminated", .func = test_parse_string_unterminated},
    {.name = "test_parse_unexpected_character", .func = test_parse_unexpected_character},
    {.name = "test_parse_value_array_object_whitespace", .func = test_parse_value_array_object_whitespace},
    {.name = "test_parse_value_array_type", .func = test_parse_value_array_type},
    {.name = "test_parse_value_array_values", .func = test_parse_value_array_values},
    {.name = "test_parse_value_booleans_null", .func = test_parse_value_booleans_null},
    {.name = "test_parse_value_number_and_string", .func = test_parse_value_number_and_string},
    {.name = "test_print_array_token", .func = test_print_array_token},
    {.name = "test_print_array_with_array_element", .func = test_print_array_with_array_element},
    {.name = "test_print_array_with_object_element", .func = test_print_array_with_object_element},
    {.name = "test_print_false_token", .func = test_print_false_token},
    {.name = "test_print_null_token", .func = test_print_null_token},
    {.name = "test_print_number_token", .func = test_print_number_token},
    {.name = "test_print_object_token", .func = test_print_object_token},
    {.name = "test_print_object_with_pair_array_value", .func = test_print_object_with_pair_array_value},
    {.name = "test_print_object_with_pair_object_value", .func = test_print_object_with_pair_object_value},
    {.name = "test_print_pair_token", .func = test_print_pair_token},
    {.name = "test_print_string_token", .func = test_print_string_token},
    {.name = "test_print_true_token", .func = test_print_true_token},
};

static int run_test_by_index(int idx)
{
    struct CMUnitTest t = {.name          = all_tests[idx].name,
                           .test_func     = all_tests[idx].func,
                           .setup_func    = NULL,
                           .teardown_func = NULL,
                           .initial_state = NULL};
    /* Pass an actual array so the cmocka_run_group_tests macro can
     * compute the number of tests with sizeof(...)/sizeof(...[0]).
     */
    struct CMUnitTest tests[] = {t};
    return cmocka_run_group_tests(tests, NULL, NULL);
}

static int run_test_by_name(const char *name)
{
    for (size_t i = 0; i < sizeof(all_tests) / sizeof(all_tests[0]); ++i)
    {
        if (strcmp(all_tests[i].name, name) == 0)
        {
            return run_test_by_index((int)i);
        }
    }
    fprintf(stderr, "Unknown test: %.256s\n", name);
    return 2;
}

static void list_tests(void)
{
    for (size_t i = 0; i < sizeof(all_tests) / sizeof(all_tests[0]); ++i)
    {
        puts(all_tests[i].name);
    }
}

int main(int argc, char **argv)
{
    if (argc == 2 && strcmp(argv[1], "--list") == 0)
    {
        list_tests();
        return 0;
    }
    if (argc == 3 && strcmp(argv[1], "--run") == 0)
    {
        return run_test_by_name(argv[2]);
    }

    /* Default: run all tests by invoking each test individually.
     * Using run_test_by_index ensures we pass an actual array to
     * cmocka_run_group_tests (via the helper) so cmocka correctly
     * computes the number of tests. The previous attempt to build a
     * heap-allocated array could end up running 0 tests because of how
     * the cmocka macro infers the group size at compile-time.
     */
    /* Build a compile-time array of CMUnitTest entries and run them as a
     * single group. This lets cmocka determine the number of tests at
     * compile-time via sizeof(group_tests)/sizeof(group_tests[0]). We
     * explicitly list each test with the cmocka_unit_test macro.
     */
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
        cmocka_unit_test(test_parse_simple_array),
        cmocka_unit_test(test_parse_simple_value),
        cmocka_unit_test(test_parse_simple),
        cmocka_unit_test(test_parse_string_empty),
        cmocka_unit_test(test_parse_string_escapes_backslash_and_slash),
        cmocka_unit_test(test_parse_string_escapes_quotes),
        cmocka_unit_test(test_parse_string_escapes),
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
        cmocka_unit_test(test_print_false_token),
        cmocka_unit_test(test_print_null_token),
        cmocka_unit_test(test_print_number_token),
        cmocka_unit_test(test_print_object_token),
        cmocka_unit_test(test_print_object_with_pair_array_value),
        cmocka_unit_test(test_print_object_with_pair_object_value),
        cmocka_unit_test(test_print_string_token),
        cmocka_unit_test(test_print_true_token),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
