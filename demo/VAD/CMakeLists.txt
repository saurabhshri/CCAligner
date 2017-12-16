cmake_minimum_required(VERSION 3.0.2)
project(vadDemo)

include(../../src/lib_ccaligner.cmake)

set(CMAKE_CXX_STANDARD 11)

if(UNIX)
    set (EXTRA_FLAGS ${EXTRA_FLAGS} -lpthread)
endif(UNIX)

#including CCAligner libraries
include_directories(../../src/lib_ccaligner/)
add_executable(vadDemo main.cpp)
target_link_libraries(vadDemo libccaligner webRTC ${EXTRA_FLAGS})
