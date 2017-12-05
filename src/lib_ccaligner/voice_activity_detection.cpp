/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "voice_activity_detection.h"

void performVAD(std::vector<int16_t>& sample)
{
    VadInst* vad = WebRtcVad_Create();  //Creating VAD handle
    if (!vad)
    {
        throw std::runtime_error("Can't create WebRTC VAD handle.");
    }

    int error = WebRtcVad_Init(vad);    //Initializing VAD handle
    if (error)
    {
        throw std::runtime_error("Can't initialize WebRTC VAD handle.");
    }

    const int aggressiveness = 2;       //Aggressiveness : 1/2/3 . Higher means higher cut-off.
    error = WebRtcVad_set_mode(vad, aggressiveness);
    if (error)
    {
        throw std::runtime_error("Can't set WebRTC VAD aggressiveness.");
    }

    const int16_t * temp = sample.data();   //16 bit ,16KHz, mono PCM samples

    for(int i=0, ms =0;i<sample.size();i+=160, ms+=10)  // currently checking in 10ms frames, most likely to change
    {
        int isActive = WebRtcVad_Process(vad, 16000, temp, 160);    // 1 = voice , 0 = not voice
        std::cout<<ms<<" ms : "<<isActive<<std::endl;
        temp = temp + 160;

    }

    WebRtcVad_Free(vad);

}
