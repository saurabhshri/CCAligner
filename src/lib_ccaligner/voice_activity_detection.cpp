/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "voice_activity_detection.h"

void performVAD(std::vector<int16_t>& sample)
{

    VadInst* vad = WebRtcVad_Create();
    if (!vad)
    {
        throw std::runtime_error("Can't create WebRTC VAD handle.");
    }

    int error = WebRtcVad_Init(vad);
    if (error)
    {
        throw std::runtime_error("Can't initialize WebRTC VAD handle.");
    }

    const int aggressiveness = 0;
    error = WebRtcVad_set_mode(vad, aggressiveness);
    if (error)
    {
        throw std::runtime_error("Can't set WebRTC VAD aggressiveness.");
    }
    std::cout<<sample.size()<<"\n\n";
    const int16_t * temp = sample.data();
    for(int i=0, ms =0;i<sample.size();i+=160, ms++)
    {
        int isActive = WebRtcVad_Process(vad, 16000, temp, 160);
        std::cout<<ms<<" ms : "<<isActive<<std::endl;
        temp = temp + 160;

    }

    bool isActive = WebRtcVad_Process(vad, 16000, sample.data(), sample.size()) == 1;
    std::cout<<isActive<<std::endl;


    WebRtcVad_Free(vad);

}
