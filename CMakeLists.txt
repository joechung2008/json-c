cmake_minimum_required(VERSION 3.12)
project(json_c VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Option to enable building tests; default is OFF so plain builds don't fail
option(JSON_C_BUILD_TESTS "Build json-c tests" OFF)

file(GLOB_RECURSE JSONC_LIB_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/lib/*.c
)

add_library(json_c SHARED ${JSONC_LIB_SOURCES})

if (WIN32)
    # Ensure MSVC creates an import library when building the shared library
    # even if explicit __declspec(dllexport) annotations are not present.
    set_target_properties(json_c PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    # Place runtime (DLL) and archive (import library) in the config-specific
    # binary directory so downstream projects can find them (e.g. Debug/)
    # Use generator expressions to avoid doubling the config directory (e.g. Debug/Debug)
    set_target_properties(json_c PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>
    )
endif()

target_include_directories(json_c
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src/lib
)

set_target_properties(json_c PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    POSITION_INDEPENDENT_CODE ON
)

add_executable(json-cli
    src/app/main.c
)

target_link_libraries(json-cli PRIVATE json_c)

if(JSON_C_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

install(TARGETS json_c json-cli
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
