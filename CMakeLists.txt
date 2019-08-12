cmake_minimum_required(VERSION 3.13.0)
project(event++ VERSION 0.1.0
                DESCRIPTION "A C++ wrapper to the libevent library, with a focus on HTTP"
                LANGUAGES C CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(event++
    src/http/request.cpp
)
set_property(TARGET event++ PROPERTY CXX_STANDARD 17)
target_link_libraries(event++
    PUBLIC
        ${CONAN_LIBS}
) 
target_include_directories(event++
    INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC     ${CMAKE_CURRENT_SOURCE_DIR}/include
)


## ---
## Testing
## ---
find_package(Catch2 REQUIRED)
add_executable(tests
    src/util/cache_test.cpp
)
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
target_include_directories(tests
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(tests Catch2::Catch2)
enable_testing()
include(Catch)
catch_discover_tests(tests)
