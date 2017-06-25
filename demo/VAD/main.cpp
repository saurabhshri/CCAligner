#include "voice_activity_detection.h"

void printUsage()
{
    std::cout << "\n\nUsage : \n"
        "           vadDemo <input_file> <arguments>"
        "           e.g. vadDemo input.wav"
        ""
        "           You can also pipe data into or take input from stdin. Simply : "
        "           vadDemo < input.wav OR yourprogram | vadDemo"
        "           e.g. cat input.wav | vadDemo"
        ""
        "           To use buffer while reading from stdin/pipe/strea"
        "           vadDemo -useBuffer < input.wav OR yourprogram | vadDemo -useBuffer";
}

int main(int argc, char *argv[])
{
    if(argc>2)
    {
        std::cout<<"Usage : ccaligner input.wav\n";
        return 1;
    }

    if(argc == 1)
    {
        WaveFileData * file = new WaveFileData();
        file->read();
        std::vector<int16_t> samples = file->getSamples();
        performVAD(samples);
    }

    else
    {
        std::string param(argv[1]);

        if(param == "-useBuffer")
        {
            WaveFileData * file = new WaveFileData(readStreamIntoBuffer);
            file->read();
            std::vector<int16_t> samples = file->getSamples();
            performVAD(samples);

        }

        else
        {
            WaveFileData * file = new WaveFileData(argv[1]);
            file->read();
            std::vector<int16_t> samples = file->getSamples();
            performVAD(samples);
        }
    }

    return 0;
}
