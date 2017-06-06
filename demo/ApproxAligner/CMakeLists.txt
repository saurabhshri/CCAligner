cmake_minimum_required(VERSION 3.0.2)
project(ApproxAlignerDemo)

set(CMAKE_CXX_STANDARD 11)

#including srtparser header file
include_directories(../../src/lib_ext/srtparser/ ../../src/lib_ccaligner/)

set(SOURCE_FILES main.cpp  ../../src/lib_ccaligner/generate_approx_timestamp.h ../../src/lib_ccaligner/generate_approx_timestamp.cpp)

add_executable(ApproxAlignerDemo ${SOURCE_FILES})
