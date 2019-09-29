find_package(Doxygen REQUIRED)

# Find all the public headers
get_target_property(PEMBROKE_PUBLIC_HEADER_DIR pembroke INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE PEMBROKE_PUBLIC_HEADERS ${PEMBROKE_PUBLIC_HEADER_DIR}/*.hpp)

# Doxygen configuration vars
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/doxygen)
set(DOXYGEN_INPUT_DIR ${PROJECT_SOURCE_DIR}/pembroke/include/)

# Main output of docs command
set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)

# Configure Doxyfile
SET(DOXYFILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
SET(DOXYFILE_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Create out output dir (cmake won't do it for us)
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
                   DEPENDS ${PEMBROKE_PUBLIC_HEADERS}
                   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                   MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                   COMMENT "Generating docs")

add_custom_target(Doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})