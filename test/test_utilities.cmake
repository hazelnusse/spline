include(../tools/tools.cmake)

CPMAddPackage(
  NAME Format.cmake
  URL https://api.github.com/repos/TheLartians/Format.cmake/tarball/1cd8380dd02aedf4ba858126cb7df195d8724939
  URL_HASH
    SHA256=b6b0ef799085f8482791fe83a32f803caabe33497e8424ca4ca1f9964d4dd1a8
)

CPMAddPackage(
  NAME ut
  URL https://api.github.com/repos/boost-ext/ut/tarball/febcd4191e920f84de16147a911e7c6f78866f69
  URL_HASH
    SHA256=c988d9f07bffa1fe4a58c34ccf51ac9edefb8afdd9a37e7929814141e8ca70ab
  DOWNLOAD_ONLY TRUE
)

if(ut_ADDED)
  add_library(ut INTERFACE)
  target_include_directories(ut INTERFACE ${ut_SOURCE_DIR}/include/boost)
  target_compile_features(ut INTERFACE cxx_std_20)
  set_target_properties(ut PROPERTIES CXX_EXTENSIONS OFF)
  target_compile_definitions(ut INTERFACE BOOST_UT_DISABLE_MODULE)
endif()

macro(enable_test_warnings library visibility)
  target_compile_options(
    ${library}
    ${visibility}
    $<$<NOT:$<BOOL:${TEST_INSTALLED_VERSION}>>:$<IF:$<CXX_COMPILER_ID:MSVC>,
    /W4
    /WX
    /permissive-,
    -W
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    $<IF:$<BOOL:WIN32>,
    -Wno-c++98-c++11-c++14-c++17-compat-pedantic
    -Wno-c++98-c++11-compat
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-c99-extensions
    -Wno-pedantic,
    -Werror>>>
  )
endmacro()

macro(spline_component_test component_source_file)
  get_filename_component(component ${component_source_file} NAME_WLE)
  get_filename_component(component_dir ${component_source_file} DIRECTORY)
  set(component_test_file ${component_dir}/${component}.test.cpp)
  set(test ${component}.test)
  add_executable(${test} ${component_test_file})
  target_link_libraries(${test} PRIVATE spline::spline ut)
  target_compile_features(${test} PRIVATE cxx_std_20)
  set_target_properties(${test} PROPERTIES CXX_EXTENSIONS OFF)
  add_test(NAME "Component ${test}" COMMAND ${test})

  # the corresponding header file.
  add_library(${component}_component OBJECT ${component_source_file})
  target_link_libraries(${component}_component PRIVATE spline::spline)
  target_compile_features(${component}_component PRIVATE cxx_std_17)

  enable_test_warnings(${test} PRIVATE)

  target_compile_options(
    ${test} PUBLIC $<$<BOOL:${ENABLE_TEST_COVERAGE}>:-O0 -g --coverage>
  )
  target_link_options(
    ${test} PUBLIC $<$<BOOL:${ENABLE_TEST_COVERAGE}>:--coverage>
  )
endmacro()
