find_package(Doxygen REQUIRED)
find_package(Sphinx REQUIRED)

# Find all the public headers
get_target_property(PEMBROKE_PUBLIC_HEADER_DIR pembroke INTERFACE_INCLUDE_DIRECTORIES)
file(GLOB_RECURSE PEMBROKE_PUBLIC_HEADERS ${PEMBROKE_PUBLIC_HEADER_DIR}/*.hpp)

## ----------------------------------------------------------------------------
## Doxygen Configuration
##

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

# Nice name to easily generate the docs
add_custom_target(doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

## ----------------------------------------------------------------------------
## Sphinx Configuration
##

set(SPHINX_SOURCE ${CMAKE_CURRENT_SOURCE_DIR})
set(SPHINX_BUILD ${CMAKE_CURRENT_BINARY_DIR}/docs/sphinx)
set(SPHINX_INDEX_FILE ${SPHINX_BUILD}/index.html)

# Only regenerate Sphinx when:
#   - Doxygen has rerun
#   - Our doc/* files have been updated (that we care about)
#   - The Sphinx config has been updated
file(GLOB_RECURSE DOC_ReST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.rst)
add_custom_command(OUTPUT ${SPHINX_INDEX_FILE}
                  COMMAND ${SPHINX_EXECUTABLE} -b html -Dbreathe_projects.Pembroke=${DOXYGEN_OUTPUT_DIR}/xml
                                               ${SPHINX_SOURCE} ${SPHINX_BUILD}
                  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                  DEPENDS
                    ${DOC_ReST_FILES}
                    ${DOXYGEN_INDEX_FILE}
                  MAIN_DEPENDENCY ${SPHINX_SOURCE}/conf.py
                  COMMENT "Generating documentation with Sphinx")

# Nice name to easily generate the docs
add_custom_target(sphinx ALL DEPENDS ${SPHINX_INDEX_FILE})


## ----------------------------------------------------------------------------
## Add an install target
##

include(GNUInstallDirs)
install(DIRECTORY ${SPHINX_BUILD}
        DESTINATION ${CMAKE_INSTALL_DOCDIR})