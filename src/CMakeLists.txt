cmake_minimum_required(VERSION 3.0.2)
project(ccaligner)

option(enable_coverage "Enable coverage report" OFF)

include(lib_ccaligner.cmake)

if(enable_coverage)
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0")
    set(EXTRA_FLAGS "${EXTRA_FLAGS} --coverage")
endif()

add_executable(ccaligner ccaligner_main.cpp)
target_link_libraries(ccaligner libccaligner webRTC pocketsphinx ${EXTRA_FLAGS})

include_directories(../test/src)

set(TEST_SOURCE_FILES
        ../test/src/output_test.cpp
        ../test/src/params.cpp
        )
add_executable(ccaligner_test ${TEST_SOURCE_FILES})

target_link_libraries(ccaligner_test libccaligner webRTC pocketsphinx gtest gtest_main ${EXTRA_FLAGS})

