if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()
######## INCLUDING LIBRARIES ########

#including subtitle parser library (srtparser.h)
include_directories(${CMAKE_CURRENT_LIST_DIR}/lib_ext/srtparser/)

#including WebRTC library(for VAD)
include_directories(${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/)
set(webRTCVADFiles
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/cross_correlation.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/division_operations.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/downsample_fast.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/energy.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/get_scaling_square.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/min_max_operations.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_48khz.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_by_2_internal.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_fractional.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_init.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_inl.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/signal_processing/vector_scaling_operations.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/vad/vad_core.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/vad/vad_filterbank.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/vad/vad_gmm.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/vad/vad_sp.c
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/webrtc/webrtc/common_audio/vad/webrtc_vad.c
)
add_library(webRTC ${webRTCVADFiles})
set_target_properties(webRTC PROPERTIES FOLDER lib_ext)

if(UNIX)
    set (EXTRA_FLAGS ${EXTRA_FLAGS} -lpthread -pthread)
endif(UNIX)

#including PocketSphinx and Sphinxbase
include_directories(${CMAKE_CURRENT_LIST_DIR}/lib_ext/pocketsphinx/include/
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/sphinxbase/include/
        ${CMAKE_CURRENT_LIST_DIR}/lib_ext/pocketsphinx/src/libpocketsphinx/
)

FILE(GLOB_RECURSE POCKETSPHINX_FILES "${CMAKE_CURRENT_LIST_DIR}/lib_ext/pocketsphinx/src/libpocketsphinx/*.c")
add_library(pocketsphinx ${POCKETSPHINX_FILES})
set_target_properties(pocketsphinx PROPERTIES FOLDER lib)


FILE(GLOB_RECURSE SPHINXBASE_FILES "${CMAKE_CURRENT_LIST_DIR}/lib_ext/sphinxbase/src/libsphinxbase/*.c")
add_library(sphinxbase ${SPHINXBASE_FILES})
set_target_properties(sphinxbase PROPERTIES FOLDER lib)

#including gtest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/lib_ext/gtest gtest.out)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#including json
include_directories(${CMAKE_CURRENT_LIST_DIR}/lib_ext/json/src/)

if(UNIX)
    add_definitions(-DHAVE_UNISTD_H)
endif(UNIX)

target_link_libraries(pocketsphinx sphinxbase)

#including CCAligner libraries
include_directories(${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/)

set(SOURCE_FILES
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/ccaligner.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/ccaligner.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/commons.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/commons.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/generate_approx_timestamp.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/generate_approx_timestamp.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/voice_activity_detection.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/voice_activity_detection.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/read_wav_file.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/read_wav_file.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/grammar_tools.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/grammar_tools.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/recognize_using_pocketsphinx.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/recognize_using_pocketsphinx.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/params.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/params.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/phoneme_utils.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/phoneme_utils.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/output_handler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/output_handler.h
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/logger.cpp
        ${CMAKE_CURRENT_LIST_DIR}/lib_ccaligner/logger.h
        )
add_library(libccaligner STATIC ${SOURCE_FILES})