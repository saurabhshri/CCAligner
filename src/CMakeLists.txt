cmake_minimum_required(VERSION 3.6)
project(ccaligner)

set(CMAKE_CXX_STANDARD 11)

#including srtparser header file
include_directories(lib_ext/srtparser/ lib_ccaligner/)

set(SOURCE_FILES ccaligner.cpp lib_ccaligner/generate_approx_timestamp.cpp lib_ccaligner/generate_approx_timestamp.h)

add_executable(ccaligner ${SOURCE_FILES})
