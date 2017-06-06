cmake_minimum_required(VERSION 3.6)
project(demo)

set(CMAKE_CXX_STANDARD 11)

include_directories(../)

set(SOURCE_FILES main.cpp ../srtparser.h)
add_executable(demo ${SOURCE_FILES})
