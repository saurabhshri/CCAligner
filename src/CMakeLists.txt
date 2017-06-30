cmake_minimum_required(VERSION 3.0.2)
project(ccaligner)

set(CMAKE_CXX_STANDARD 11)

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
    set (EXTRA_FLAGS ${EXTRA_FLAGS} -lpthread)
endif(UNIX)

#including CCAligner libraries
include_directories(lib_ccaligner/)

set(SOURCE_FILES
        ccaligner.cpp
        lib_ccaligner/generate_approx_timestamp.cpp
        lib_ccaligner/generate_approx_timestamp.h
        lib_ccaligner/voice_activity_detection.h
        lib_ccaligner/voice_activity_detection.cpp
        lib_ccaligner/common_header_files.h
        lib_ccaligner/read_wav_file.h
        lib_ccaligner/read_wav_file.cpp
        ccaligner.h)

add_executable(ccaligner ${SOURCE_FILES})
target_link_libraries(ccaligner webRTC ${EXTRA_FLAGS})
