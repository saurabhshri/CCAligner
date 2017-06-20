//
// Created by Saurabh on 07-06-2017.
//

#ifndef VOICE_ACTIVITY_DETECTION_H
#define VOICE_ACTIVITY_DETECTION_H

#include "read_wav_file.h"
#include <webrtc/common_audio/vad/include/webrtc_vad.h>
#include "common_header_files.h"

void performVAD(std::vector<int16_t>& sample);

#endif //VOICE_ACTIVITY_DETECTION_H
