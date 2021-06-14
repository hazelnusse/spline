cmake_minimum_required(VERSION 3.17...3.20 FATAL_ERROR)
project(de_casteljau_test LANGUAGES CXX)
enable_testing()

option(ENABLE_TEST_COVERAGE "Enable test coverage" OFF)
option(TEST_INSTALLED_VERSION "Test the version found by find_package" OFF)

include(test_utilities.cmake)
if(TEST_INSTALLED_VERSION)
  find_package(spline REQUIRED)
else()
  CPMAddPackage(NAME spline SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)
endif()

set(component_files
    "${CMAKE_CURRENT_LIST_DIR}/../source/concepts.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../source/de_casteljau_subdivide.cpp"
)

enable_test_warnings(spline INTERFACE)

foreach(file ${component_files})
  spline_component_test(${file})
endforeach()
