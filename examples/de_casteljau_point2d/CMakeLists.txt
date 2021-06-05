cmake_minimum_required(VERSION 3.17...3.20 FATAL_ERROR)
project(de_casteljau_point2d LANGUAGES CXX)

include(../../tools/CPM.cmake)
CPMAddPackage(NAME spline SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../..)

add_executable(de_casteljau_point2d main.cpp)
set_target_properties(de_casteljau_point2d PROPERTIES CXX_STANDARD 20)
target_link_libraries(de_casteljau_point2d spline::spline)
