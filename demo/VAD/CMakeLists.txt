cmake_minimum_required(VERSION 3.0.2)
project(vadDemo)

set(CMAKE_CXX_STANDARD 11)

######## INCLUDING LIBRARIES ########

#including subtitle parser library (srtparser.h)
include_directories(../../src/lib_ext/srtparser/)

#including WebRTC library(for VAD)
include_directories(../../src/lib_ext/webrtc/)
set(webRTCVADFiles
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/cross_correlation.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/division_operations.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/downsample_fast.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/energy.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/get_scaling_square.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/min_max_operations.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_48khz.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_by_2_internal.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/resample_fractional.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_init.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/spl_inl.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/signal_processing/vector_scaling_operations.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/vad/vad_core.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/vad/vad_filterbank.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/vad/vad_gmm.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/vad/vad_sp.c
       ../../src/lib_ext/webrtc/webrtc/common_audio/vad/webrtc_vad.c
        )
add_library(webRTC ${webRTCVADFiles})
set_target_properties(webRTC PROPERTIES FOLDER lib_ext)

if(UNIX)
    set (EXTRA_FLAGS ${EXTRA_FLAGS} -lpthread)
endif(UNIX)

#including CCAligner libraries
include_directories(../../src/lib_ccaligner/)

set(SOURCE_FILES
        main.cpp
        ../../src/lib_ccaligner/voice_activity_detection.h
        ../../src/lib_ccaligner/voice_activity_detection.cpp
        ../../src/lib_ccaligner/common_header_files.h
        ../../src/lib_ccaligner/read_wav_file.h
        ../../src/lib_ccaligner/read_wav_file.cpp
        )

add_executable(vadDemo ${SOURCE_FILES})
target_link_libraries(vadDemo webRTC ${EXTRA_FLAGS})
