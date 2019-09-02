## ---
## Basic Project Setup
## ---
cmake_minimum_required(VERSION 3.13.0)
project(pembroke VERSION 0.1.0
                 DESCRIPTION "A C++ wrapper to the libevent library, with a focus on HTTP"
                 LANGUAGES C CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

## ---
## Pembroke Library Definition
## ---
add_library(pembroke
    src/event.cpp
    src/http/request.cpp
    src/internal/logging.cpp
    src/reactor.cpp
    src/scheduler.cpp
    src/task.cpp
)
set_property(TARGET pembroke PROPERTY CXX_STANDARD 17)
target_link_libraries(pembroke
    PUBLIC
        ${CONAN_LIBS}
) 
target_include_directories(pembroke
    INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC     ${CMAKE_CURRENT_SOURCE_DIR}/include
)


## ---
## Testing
## ---
find_package(Catch2 REQUIRED)
add_executable(tests
    src/main_test.cpp

    src/event_test.cpp
    src/internal/logging_test.cpp
    src/reactor_test.cpp
    src/task_test.cpp
)
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
target_link_libraries(tests 
    Catch2::Catch2
    pembroke
)
enable_testing()
include(Catch)
catch_discover_tests(tests)
