## ---
## Setup/Include Dependencies (Conan)
## ---
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

## ---
## Pembroke Library Definition
## ---
add_library(pembroke
    src/buffer.cpp
    src/event.cpp
    src/event/delayed.cpp
    src/event/timer.cpp
    src/http/request.cpp
    src/internal/logging.cpp
    src/internal/util.cpp
    src/reactor.cpp
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

    src/buffer_test.cpp
    src/event/delayed_test.cpp
    src/internal/logging_test.cpp
    src/internal/util_test.cpp
    src/reactor_test.cpp

    src/internal/test_common.cpp
)
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
target_link_libraries(tests 
    Catch2::Catch2
    pembroke
)
include(Catch)
catch_discover_tests(tests)
