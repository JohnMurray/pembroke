if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(FATAL_ERROR "TSAN cannot be built in Release configuration")
endif()

macro(CHECK_TSAN COMPILER_ID TSAN_FLAGS)
    if ("${COMPILER_ID}" STREQUAL "Clang")
        set(${TSAN_FLAGS} "-fsanitize=thread -fno-omit-frame-pointer")
    else()
        message(WARNING "Tsan not supported for ${CMAKE_CXX_COMPILER_ID} compiler")
    endif()
endmacro()

CHECK_TSAN(${CMAKE_C_COMPILER_ID} TSAN_C_FLAGS)
CHECK_TSAN(${CMAKE_CXX_COMPILER_ID} TSAN_CXX_FLAGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TSAN_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TSAN_CXX_FLAGS}")