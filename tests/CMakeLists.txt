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
