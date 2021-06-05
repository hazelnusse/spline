cmake_minimum_required(VERSION 3.17...3.20 FATAL_ERROR)

project(spline_documentation)
include(../tools/CPM.cmake)

CPMAddPackage("gh:mosra/m.css#9385194fa3392a7162e7535cc2478814e382ff8a")
CPMAddPackage(NAME spline SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

set(DOXYGEN_PROJECT_NAME "Spline")
set(DOXYGEN_PROJECT_VERSION ${spline_VERSION})
set(DOXYGEN_PROJECT_ROOT "${CMAKE_CURRENT_LIST_DIR}/..")
set(DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/doxygen")

configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
)

configure_file(
  ${CMAKE_CURRENT_LIST_DIR}/conf.py ${CMAKE_CURRENT_BINARY_DIR}/conf.py
)

add_custom_target(
  GenerateDocs
  ${CMAKE_COMMAND} -E make_directory "${DOXYGEN_OUTPUT_DIRECTORY}"
  COMMAND "${m.css_SOURCE_DIR}/documentation/doxygen.py" "--debug"
          "${CMAKE_CURRENT_BINARY_DIR}/conf.py"
  COMMAND ${CMAKE_COMMAND} -E echo
          "Docs written to: ${DOXYGEN_OUTPUT_DIRECTORY}"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
)
