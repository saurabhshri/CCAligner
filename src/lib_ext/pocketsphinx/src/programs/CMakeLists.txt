cmake_minimum_required(VERSION 3.0.2)
project(prog)

set(CMAKE_CXX_STANDARD 11)

#including PocketSphinx and Sphinxbase
include_directories(../../../pocketsphinx/include/
        ../../../sphinxbase/include/
)

FILE(GLOB POCKETSPHINX_FILES "../../../pocketsphinx/src/libpocketsphinx/*.c")
add_library(pocketsphinx ${POCKETSPHINX_FILES})
target_compile_definitions(pocketsphinx PUBLIC __POCKETSPHINX_EXPORT_H__=1 POCKETSPHINX_EXPORT=)
set_target_properties(pocketsphinx PROPERTIES FOLDER lib)


FILE(GLOB_RECURSE SPHINXBASE_FILES "../../../sphinxbase/src/libsphinxbase/*.c")
add_library(sphinxbase ${SPHINXBASE_FILES})
target_compile_definitions(sphinxbase PUBLIC __SPHINXBASE_EXPORT_H__=1 SPHINXBASE_EXPORT=) # Compile as static lib
set_target_properties(sphinxbase PROPERTIES FOLDER lib)

add_executable(prog continuous.c)
target_link_libraries(prog pocketsphinx sphinxbase)
