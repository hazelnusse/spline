#!/bin/bash

build_dir='build_all_configurations'
wipe_build_dir=false

usage () { echo "$0 [-d build_directory] [-w] [-h]"; }

while getopts ':d:wh' opt; do
  case ${opt} in
    d ) build_dir=$OPTARG;;
    w ) wipe_build_dir=true;;
    h ) usage; exit;;
    \? ) echo "Unknown option: -$OPTARG" >&2; exit 1;;
    :  ) echo "Missing option argument for -$OPTARG" >&2; exit 1;;
    *  ) echo "Unimplemented option: -$OPTARG" >&2; exit 1;;
  esac
done

if [ "$wipe_build_dir" = true ]; then
    rm -rf $build_dir
fi

mkdir -p $build_dir

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

# Configure, build, and install parent project.
echo "Building top level project"
parent_build=$build_dir/parent
cmake -S $DIR -B $parent_build/build -DCMAKE_INSTALL_PREFIX=$parent_build/install
cmake --build $parent_build/build
cmake --build $parent_build/build --target install

# Configure, build, install everything.
echo "Building all"
all_build=$build_dir/all
cmake -S $DIR/all -B $all_build/build -DCMAKE_INSTALL_PREFIX=$all_build/install
cmake --build $all_build/build
cmake --build $all_build/build --target install
$all_build/build/test/SplineTests
cmake --build build/test --target format

# Configure, build, all, address sanitizer.
asan_build=$build_dir/asan
cmake -S $DIR/all -B $asan_build/build -DUSE_SANITIZER=Address
cmake --build $asan_build/build --target all
$asan_build/build/test/SplineTests

# # Configure, build, all, memory sanitizer.
# memory_build=$build_dir/memory
# cmake -S $DIR/all -B $memory_build/build -DUSE_SANITIZER=Memory
# cmake --build $memory_build/build --target all
# $memory_build/build/test/SplineTests
# 
# # Configure, build, all, memory with origins sanitizer.
# memorywithorigins_build=$build_dir/memorywithorigins
# cmake -S $DIR/all -B $memorywithorigins_build/build -DUSE_SANITIZER=MemoryWithOrigins
# cmake --build $memorywithorigins_build/build --target all
# $memorywithorigins_build/build/test/SplineTests

# Configure, build, all, undefined sanitizer.
undefined_build=$build_dir/undefined
cmake -S $DIR/all -B $undefined_build/build -DUSE_SANITIZER=Undefined
cmake --build $undefined_build/build --target all
$undefined_build/build/test/SplineTests

# Configure, build, all, thread sanitizer.
thread_build=$build_dir/thread
cmake -S $DIR/all -B $thread_build/build -DUSE_SANITIZER=Thread
cmake --build $thread_build/build --target all
$thread_build/build/test/SplineTests

# Configure, build, all, leak sanitizer.
leak_build=$build_dir/leak
cmake -S $DIR/all -B $leak_build/build -DUSE_SANITIZER=Leak
cmake --build $leak_build/build --target all
$leak_build/build/test/SplineTests

# Configure, build all, clang-tidy
tidy_build=$build_dir/tidy
cmake -S $DIR/all -B $tidy_build/build -DUSE_STATIC_ANALYZER=clang-tidy
cmake --build $tidy_build/build --target all
$tidy_build/build/test/SplineTests

# Configure, build all, iwyu
iwyu_build=$build_dir/iwyu
cmake -S $DIR/all -B $iwyu_build/build -DUSE_STATIC_ANALYZER=iwyu
cmake --build $iwyu_build/build --target all
$iwyu_build/build/test/SplineTests

# Configure, build all, cppcheck
cppcheck_build=$build_dir/cppcheck
cmake -S $DIR/all -B $cppcheck_build/build -DUSE_STATIC_ANALYZER=cppcheck -DCMAKE_CXX_CPPCHECK="cppcheck;--force"
cmake --build $cppcheck_build/build --target all
$cppcheck_build/build/test/SplineTests
