## ---
## Setup/Include Dependencies (Conan)
## ---
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

## ---
## Pembroke Library Definition
## ---
add_library(pembroke
    src/pembroke/buffer.cpp
    src/pembroke/event/delayed.cpp
    src/pembroke/event/timer.cpp
    src/pembroke/http/request.cpp
    src/pembroke/internal/logging.cpp
    src/pembroke/internal/util.cpp
    src/pembroke/reactor.cpp
)
set_property(TARGET pembroke PROPERTY CXX_STANDARD 17)
target_link_libraries(pembroke
    PUBLIC
        ${CONAN_LIBS}
) 
target_include_directories(pembroke
    INTERFACE  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC     ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE    ${CMAKE_CURRENT_SOURCE_DIR}/src
)


## ---
## Testing
## ---
find_package(Catch2 REQUIRED)
add_executable(tests
    src/pembroke/main_test.cpp

    src/pembroke/buffer_test.cpp
    src/pembroke/event/delayed_test.cpp
    src/pembroke/event/timer_test.cpp
    src/pembroke/internal/logging_test.cpp
    src/pembroke/internal/util_test.cpp
    src/pembroke/reactor_test.cpp

    src/pembroke/internal/test_common.cpp
)
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
target_link_libraries(tests 
    Catch2::Catch2
    pembroke
)
target_include_directories(tests
    # allow tests to use internal headers
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
include(Catch)
catch_discover_tests(tests)
