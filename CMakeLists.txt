cmake_minimum_required(VERSION 3.12)
project(json_c VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE JSONC_LIB_SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/lib/*.c
)

add_library(json_c SHARED ${JSONC_LIB_SOURCES})

target_include_directories(json_c
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
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

enable_testing()
add_subdirectory(tests)

install(TARGETS json_c json-cli
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)
