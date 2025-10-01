cmake_minimum_required(VERSION 3.12)

include(FetchContent)

include(CheckCSourceCompiles)

# Try pkg-config first
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(CMOCKA QUIET cmocka)
endif()

if(NOT CMOCKA_FOUND)
    # Try to find a system library
    find_library(CMOCKA_LIB NAMES cmocka)
    if(CMOCKA_LIB)
        set(CMOCKA_FOUND TRUE)
    endif()
endif()

if(NOT CMOCKA_FOUND)
    message(STATUS "cmocka not found - fetching cmocka with FetchContent")
    FetchContent_Declare(
        cmocka
        GIT_REPOSITORY https://github.com/clibs/cmocka.git
        GIT_TAG 1.1.5
    )
    # Disable building cmocka's own examples and internal tests when fetched
    # so FetchContent doesn't create extra ctest entries for example tests.
    set(UNIT_TESTING OFF CACHE BOOL "Disable cmocka internal unit tests" FORCE)
    set(WITH_EXAMPLES OFF CACHE BOOL "Disable cmocka example builds" FORCE)
    FetchContent_MakeAvailable(cmocka)
    # cmocka provides a target named cmocka
    set(CMOCKA_LIB cmocka)
else()
    # If pkg-config defined include/lib vars, prefer them, otherwise use found lib
    if(DEFINED CMOCKA_LIBRARIES)
        set(CMOCKA_LIB ${CMOCKA_LIBRARIES})
    endif()
endif()

add_executable(json_tests test_json_parse.c)
target_sources(json_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test_array.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_number.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_object.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_pair.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_value.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_printers_value.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_printers_number.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_printers_string.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_printers_array.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_printers_object.c
    ${CMAKE_CURRENT_SOURCE_DIR}/test_printers_pair.c
)

target_include_directories(json_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(json_tests PRIVATE json_c ${CMOCKA_LIB})

# Prefer C17 for tests, fall back to C11 when C17 isn't available on the toolchain.
# Use CheckCSourceCompiles to probe whether the compiler accepts -std=c17/-std=c11.
set(_old_CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
set(CMAKE_REQUIRED_FLAGS "-std=c17")
check_c_source_compiles("int main(void){return 0;}" HAVE_C17)
set(CMAKE_REQUIRED_FLAGS "${_old_CMAKE_REQUIRED_FLAGS}")
if(HAVE_C17)
    set(TEST_C_STD 17)
else()
    set(CMAKE_REQUIRED_FLAGS "-std=c11")
    check_c_source_compiles("int main(void){return 0;}" HAVE_C11)
    set(CMAKE_REQUIRED_FLAGS "${_old_CMAKE_REQUIRED_FLAGS}")
    if(HAVE_C11)
        set(TEST_C_STD 11)
    else()
        message(WARNING "Neither C17 nor C11 could be validated by the compiler; falling back to the project's default C standard.")
    endif()
endif()

if(DEFINED TEST_C_STD)
    set_target_properties(json_tests PROPERTIES
        C_STANDARD ${TEST_C_STD}
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
    )
endif()

# Enable strict warnings and treat warnings as errors for test builds.
if(MSVC)
    target_compile_options(json_tests PRIVATE /W4 /WX)
else()
    target_compile_options(json_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

# Ensure the test can find the shared library at runtime when run from the build tree.
# On Unix use RPATH ($ORIGIN). On macOS use @loader_path. On Windows copy the DLL
# next to the test executable so the loader finds it.
if(WIN32)
    add_custom_command(TARGET json_tests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:json_c>
            $<TARGET_FILE_DIR:json_tests>
    )
elseif(APPLE)
    set_target_properties(json_tests PROPERTIES BUILD_RPATH "@loader_path")
else()
    set_target_properties(json_tests PROPERTIES BUILD_RPATH "\$ORIGIN")
endif()

add_test(NAME json_tests COMMAND json_tests)

# Create individual parser test executables so each is a separate CTest entry
add_test(NAME test_parse_array_and_values COMMAND json_tests --run test_parse_array_and_values)
add_test(NAME test_parse_array_empty_and_numbers COMMAND json_tests --run test_parse_array_empty_and_numbers)
add_test(NAME test_parse_array_empty_type COMMAND json_tests --run test_parse_array_empty_type)
add_test(NAME test_parse_array_missing_opening_bracket COMMAND json_tests --run test_parse_array_missing_opening_bracket)
add_test(NAME test_parse_array_mixed_types COMMAND json_tests --run test_parse_array_mixed_types)
add_test(NAME test_parse_array_nested_and_mixed COMMAND json_tests --run test_parse_array_nested_and_mixed)
add_test(NAME test_parse_array_nested_first_element COMMAND json_tests --run test_parse_array_nested_first_element)
add_test(NAME test_parse_array_nested_second_element COMMAND json_tests --run test_parse_array_nested_second_element)
add_test(NAME test_parse_array_of_strings_and_booleans COMMAND json_tests --run test_parse_array_of_strings_and_booleans)
add_test(NAME test_parse_array_unexpected_delimiter COMMAND json_tests --run test_parse_array_unexpected_delimiter)
add_test(NAME test_parse_empty_input COMMAND json_tests --run test_parse_empty_input)
add_test(NAME test_parse_empty_object COMMAND json_tests --run test_parse_empty_object)
add_test(NAME test_parse_invalid_input COMMAND json_tests --run test_parse_invalid_input)
add_test(NAME test_parse_malformed_false_literal COMMAND json_tests --run test_parse_malformed_false_literal)
add_test(NAME test_parse_malformed_null_literal COMMAND json_tests --run test_parse_malformed_null_literal)
add_test(NAME test_parse_malformed_true_literal COMMAND json_tests --run test_parse_malformed_true_literal)
add_test(NAME test_parse_multiple_pairs_and_commas COMMAND json_tests --run test_parse_multiple_pairs_and_commas)
add_test(NAME test_parse_number_decimal_and_exponent COMMAND json_tests --run test_parse_number_decimal_and_exponent)
add_test(NAME test_parse_number_integer_and_types COMMAND json_tests --run test_parse_number_integer_and_types)
add_test(NAME test_parse_number_invalid_cases COMMAND json_tests --run test_parse_number_invalid_cases)
add_test(NAME test_parse_number_positive_and_negative COMMAND json_tests --run test_parse_number_positive_and_negative)
add_test(NAME test_parse_number_simple COMMAND json_tests --run test_parse_number_simple)
add_test(NAME test_parse_object_empty COMMAND json_tests --run test_parse_object_empty)
add_test(NAME test_parse_object_multiple_pairs_and_keys COMMAND json_tests --run test_parse_object_multiple_pairs_and_keys)
add_test(NAME test_parse_object_nested_and_mixed COMMAND json_tests --run test_parse_object_nested_and_mixed)
add_test(NAME test_parse_object_single_and_multiple COMMAND json_tests --run test_parse_object_single_and_multiple)
add_test(NAME test_parse_object_single_pair COMMAND json_tests --run test_parse_object_single_pair)
add_test(NAME test_parse_object_unexpected_delimiter COMMAND json_tests --run test_parse_object_unexpected_delimiter)
add_test(NAME test_parse_pair_errors COMMAND json_tests --run test_parse_pair_errors)
add_test(NAME test_parse_pair_key_and_value_bool COMMAND json_tests --run test_parse_pair_key_and_value_bool)
add_test(NAME test_parse_pair_key_and_value_null COMMAND json_tests --run test_parse_pair_key_and_value_null)
add_test(NAME test_parse_pair_key_and_value_number COMMAND json_tests --run test_parse_pair_key_and_value_number)
add_test(NAME test_parse_pair_key_and_value_string COMMAND json_tests --run test_parse_pair_key_and_value_string)
add_test(NAME test_parse_pair_key_value_parsing COMMAND json_tests --run test_parse_pair_key_value_parsing)
add_test(NAME test_parse_pair_simple COMMAND json_tests --run test_parse_pair_simple)
add_test(NAME test_parse_pair_value_array_and_object_types COMMAND json_tests --run test_parse_pair_value_array_and_object_types)
add_test(NAME test_parse_pair_whitespace_handling COMMAND json_tests --run test_parse_pair_whitespace_handling)
add_test(NAME test_parse_simple COMMAND json_tests --run test_parse_simple)
add_test(NAME test_parse_simple_array COMMAND json_tests --run test_parse_simple_array)
add_test(NAME test_parse_simple_value COMMAND json_tests --run test_parse_simple_value)
add_test(NAME test_parse_string_empty COMMAND json_tests --run test_parse_string_empty)
add_test(NAME test_parse_string_escapes COMMAND json_tests --run test_parse_string_escapes)
add_test(NAME test_parse_string_escapes_backslash_and_slash COMMAND json_tests --run test_parse_string_escapes_backslash_and_slash)
add_test(NAME test_parse_string_escapes_quotes COMMAND json_tests --run test_parse_string_escapes_quotes)
add_test(NAME test_parse_string_invalid_escape COMMAND json_tests --run test_parse_string_invalid_escape)
add_test(NAME test_parse_string_invalid_unicode COMMAND json_tests --run test_parse_string_invalid_unicode)
add_test(NAME test_parse_string_malformed_escape_sequences COMMAND json_tests --run test_parse_string_malformed_escape_sequences)
add_test(NAME test_parse_string_missing_opening_quote COMMAND json_tests --run test_parse_string_missing_opening_quote)
add_test(NAME test_parse_string_simple COMMAND json_tests --run test_parse_string_simple)
add_test(NAME test_parse_string_unicode COMMAND json_tests --run test_parse_string_unicode)
add_test(NAME test_parse_string_unterminated COMMAND json_tests --run test_parse_string_unterminated)
add_test(NAME test_parse_unexpected_character COMMAND json_tests --run test_parse_unexpected_character)
add_test(NAME test_parse_value_array_object_whitespace COMMAND json_tests --run test_parse_value_array_object_whitespace)
add_test(NAME test_parse_value_array_type COMMAND json_tests --run test_parse_value_array_type)
add_test(NAME test_parse_value_array_values COMMAND json_tests --run test_parse_value_array_values)
add_test(NAME test_parse_value_booleans_null COMMAND json_tests --run test_parse_value_booleans_null)
add_test(NAME test_parse_value_number_and_string COMMAND json_tests --run test_parse_value_number_and_string)

add_test(NAME test_print_array_token COMMAND json_tests --run test_print_array_token)
add_test(NAME test_print_array_with_array_element COMMAND json_tests --run test_print_array_with_array_element)
add_test(NAME test_print_array_with_object_element COMMAND json_tests --run test_print_array_with_object_element)
add_test(NAME test_print_false_token COMMAND json_tests --run test_print_false_token)
add_test(NAME test_print_null_token COMMAND json_tests --run test_print_null_token)
add_test(NAME test_print_number_token COMMAND json_tests --run test_print_number_token)
add_test(NAME test_print_object_token COMMAND json_tests --run test_print_object_token)
add_test(NAME test_print_object_with_pair_array_value COMMAND json_tests --run test_print_object_with_pair_array_value)
add_test(NAME test_print_object_with_pair_object_value COMMAND json_tests --run test_print_object_with_pair_object_value)
add_test(NAME test_print_pair_token COMMAND json_tests --run test_print_pair_token)
add_test(NAME test_print_string_token COMMAND json_tests --run test_print_string_token)
add_test(NAME test_print_true_token COMMAND json_tests --run test_print_true_token)
