cmake_minimum_required(VERSION 3.11.1)
project(event++ VERSION 0.1.0
                DESCRIPTION "A C++ wrapper to the libevent library, with a focus on HTTP"
                LANGUAGES C CXX)

add_definitions("-std=c++2a")
# add_definitions("-Wc++17-extensions")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(event++
    src/http/request.cpp
)
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
find_package(catch2 REQUIRED)
add_executable(tests
    src/util/cache_test.cpp
)
target_include_directories(tests
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(tests Catch2::Catch2)
include(Ctest)
include(Catch)
catch_discover_tests(tests)