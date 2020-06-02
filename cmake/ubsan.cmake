if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(FATAL_ERROR "UBSAN cannot be built in Release configuration")
endif()

macro(CHECK_UBSAN COMPILER_ID UBSAN_FLAGS)
    if ("${COMPILER_ID}" STREQUAL "Clang")
        set(${UBSAN_FLAGS} "-fsanitize=undefined -fno-omit-frame-pointer")
    else()
        message(WARNING "UBsan not supported for ${CMAKE_CXX_COMPILER_ID} compiler")
    endif()
endmacro()

CHECK_UBSAN(${CMAKE_C_COMPILER_ID} UBSAN_C_FLAGS)
CHECK_UBSAN(${CMAKE_CXX_COMPILER_ID} UBSAN_CXX_FLAGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${UBSAN_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${UBSAN_CXX_FLAGS}")