## ---
## Minimal Project Setup
## ---
cmake_minimum_required(VERSION 3.13.0)
project(pembroke VERSION 0.1.0
                 DESCRIPTION "A C++ wrapper to the libevent library, with a focus on HTTP"
                 LANGUAGES C CXX)
enable_testing()

# Add the cmake folder so the Find* modules are available
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory("pembroke")
add_subdirectory("docs")