//
// Created by Saurabh on 07-06-2017.
//

#ifndef VOICE_ACTIVITY_DETECTION_H
#define VOICE_ACTIVITY_DETECTION_H

#include "read_wav_file.h"
#include <webrtc/common_audio/vad/include/webrtc_vad.h>

void performVAD(std::vector<int16_t>& sample);  //use webRTC's VAD to check if a window of sample has voice.

#endif //VOICE_ACTIVITY_DETECTION_H
