cmake_minimum_required(VERSION 3.0.2)
project(ccaligner)

if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
else ()
    set (CMAKE_CXX_STANDARD 11)
endif ()

######## INCLUDING LIBRARIES ########

#including subtitle parser library (srtparser.h)
include_directories(lib_ext/srtparser/)

#including WebRTC library(for VAD)
include_directories(lib_ext/webrtc/)
set(webRTCVADFiles
        lib_ext/webrtc/webrtc/common_audio/signal_processing/cross_correlation.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/division_operations.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/downsample_fast.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/energy.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/get_scaling_square.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/min_max_operations.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_48khz.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_by_2_internal.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_fractional.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_init.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_inl.c
        lib_ext/webrtc/webrtc/common_audio/signal_processing/vector_scaling_operations.c
        lib_ext/webrtc/webrtc/common_audio/vad/vad_core.c
        lib_ext/webrtc/webrtc/common_audio/vad/vad_filterbank.c
        lib_ext/webrtc/webrtc/common_audio/vad/vad_gmm.c
        lib_ext/webrtc/webrtc/common_audio/vad/vad_sp.c
        lib_ext/webrtc/webrtc/common_audio/vad/webrtc_vad.c
)
add_library(webRTC ${webRTCVADFiles})
set_target_properties(webRTC PROPERTIES FOLDER lib_ext)

if(UNIX)
    set (EXTRA_FLAGS ${EXTRA_FLAGS} -lpthread -pthread)
endif(UNIX)

#including PocketSphinx and Sphinxbase
include_directories(lib_ext/pocketsphinx/include/
        lib_ext/sphinxbase/include/
        lib_ext/pocketsphinx/src/libpocketsphinx/
)

FILE(GLOB_RECURSE POCKETSPHINX_FILES "lib_ext/pocketsphinx/src/libpocketsphinx/*.c")
add_library(pocketsphinx ${POCKETSPHINX_FILES})
set_target_properties(pocketsphinx PROPERTIES FOLDER lib)


FILE(GLOB_RECURSE SPHINXBASE_FILES "lib_ext/sphinxbase/src/libsphinxbase/*.c")
add_library(sphinxbase ${SPHINXBASE_FILES})
set_target_properties(sphinxbase PROPERTIES FOLDER lib)

if(UNIX)
    add_definitions(-DHAVE_UNISTD_H)
endif(UNIX)

target_link_libraries(pocketsphinx sphinxbase)

#including CCAligner libraries
include_directories(lib_ccaligner/)

set(SOURCE_FILES
        ccaligner.cpp
        ccaligner.h
        lib_ccaligner/commons.cpp
        lib_ccaligner/commons.h
        lib_ccaligner/generate_approx_timestamp.cpp
        lib_ccaligner/generate_approx_timestamp.h
        lib_ccaligner/voice_activity_detection.h
        lib_ccaligner/voice_activity_detection.cpp
        lib_ccaligner/read_wav_file.h
        lib_ccaligner/read_wav_file.cpp
        lib_ccaligner/grammar_tools.cpp
        lib_ccaligner/grammar_tools.h
        lib_ccaligner/recognize_using_pocketsphinx.cpp
        lib_ccaligner/recognize_using_pocketsphinx.h
        lib_ccaligner/params.cpp
        lib_ccaligner/params.h
        lib_ccaligner/phoneme_utils.cpp
        lib_ccaligner/phoneme_utils.h
        lib_ccaligner/output_handler.cpp
        lib_ccaligner/output_handler.h
        lib_ccaligner/logger.cpp
        lib_ccaligner/logger.h
        )

add_executable(ccaligner ${SOURCE_FILES})
target_link_libraries(ccaligner webRTC  pocketsphinx sphinxbase ${EXTRA_FLAGS})
