cmake_minimum_required(VERSION 3.12)
project(json_c VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Export compile commands for clang-tidy (Linux/Mac only, avoids Windows symlink issues)
if(UNIX)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/obj)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out)

file(GLOB_RECURSE JSONC_LIB_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/lib/*.c
)

add_library(json_c STATIC ${JSONC_LIB_SOURCES})

target_include_directories(json_c
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/lib
)

add_executable(json-cli
    src/app/main.c
)

target_link_libraries(json-cli PRIVATE json_c)

enable_testing()

include(FetchContent)

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

add_executable(json_tests tests/test_json_parse.c)
target_sources(json_tests PRIVATE
    tests/test_array.c
    tests/test_number.c
    tests/test_object.c
    tests/test_pair.c
    tests/test_string.c
    tests/test_value.c
    tests/test_printers_value.c
    tests/test_printers_number.c
    tests/test_printers_string.c
    tests/test_printers_array.c
    tests/test_printers_object.c
    tests/test_printers_pair.c
)

target_link_libraries(json_tests PRIVATE json_c ${CMOCKA_LIB})

set_target_properties(json_tests PROPERTIES
    C_STANDARD 17
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

# Enable strict warnings and treat warnings as errors for test builds.
if(MSVC)
    target_compile_options(json_tests PRIVATE /W4 /WX)
else()
    target_compile_options(json_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

add_test(NAME json_tests COMMAND json_tests)

install(TARGETS json_c json-cli
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
