#include "generate_approx_timestamp.h"
#include "read_wav_file.h"
#include "voice_activity_detection.h"

int main(int argc, char *argv[])
{
	if(argc<2)
	{
		std::cout<<"Usage : ccaligner input.srt\n";
		return 1;
	}

    std::string filename(argv[1]);

    WaveFileData * file = new WaveFileData(filename);
    file->openFile();
    std::vector<int16_t> samples = file->getSamples();
    performVAD(samples);

    return 0;
}
