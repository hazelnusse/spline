cmake_minimum_required(VERSION 3.17...3.20 FATAL_ERROR)

project(
  spline
  VERSION 0.1
  LANGUAGES CXX
)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed.")
endif()

include(tools/CPM.cmake)

add_library(spline INTERFACE)
target_include_directories(
  spline
  INTERFACE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:include/${PROJECT_NAME}-${PROJECT_VERSION}>
)
# Help out IDE users
target_sources(
  spline
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/spline/de_casteljau_subdivide.hpp>
    $<INSTALL_INTERFACE:include/spline/de_casteljau_subdivide.hpp>
)

CPMAddPackage("gh:TheLartians/PackageProject.cmake@1.7.0")
packageProject(
  NAME ${PROJECT_NAME}
  VERSION ${PROJECT_VERSION}
  NAMESPACE ${PROJECT_NAME}
  BINARY_DIR ${PROJECT_BINARY_DIR}
  INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include
  INCLUDE_DESTINATION include/${PROJECT_NAME}-${PROJECT_VERSION}
  VERSION_HEADER "${PROJECT_NAME}/version.hpp"
)
