cmake_minimum_required(VERSION 3.17...3.20 FATAL_ERROR)

project(build_all LANGUAGES CXX)

include(../tools/tools.cmake)

enable_testing()

add_subdirectory(
  ${CMAKE_CURRENT_LIST_DIR}/../examples/de_casteljau_point2d
  ${CMAKE_BINARY_DIR}/examples/de_casteljau_point2d
)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../test ${CMAKE_BINARY_DIR}/test)
add_subdirectory(
  ${CMAKE_CURRENT_LIST_DIR}/../documentation ${CMAKE_BINARY_DIR}/documentation
)
