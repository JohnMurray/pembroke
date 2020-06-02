if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(FATAL_ERROR "MSAN cannot be built in Release configuration")
endif()

macro(CHECK_MSAN COMPILER_ID MSAN_FLAGS)
    if ("${COMPILER_ID}" STREQUAL "Clang")
        set(${MSAN_FLAGS} "-fsanitize=memory -fno-omit-frame-pointer")
    else()
        message(WARNING "Msan not supported for ${CMAKE_CXX_COMPILER_ID} compiler")
    endif()
endmacro()

CHECK_MSAN(${CMAKE_C_COMPILER_ID} MSAN_C_FLAGS)
CHECK_MSAN(${CMAKE_CXX_COMPILER_ID} MSAN_CXX_FLAGS)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MSAN_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${MSAN_CXX_FLAGS}")