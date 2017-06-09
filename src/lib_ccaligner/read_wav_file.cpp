/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "read_wav_file.h"

int findIndex(std::vector<unsigned char>& fileData, std::string chunk)
{
    auto it = std::search(fileData.begin(), fileData.end(), chunk.begin(), chunk.end());
    return it-fileData.begin();
}

WaveFileData::WaveFileData(std::string fileName)
{
    _fileName = fileName;
    _samples.resize(0);
}

bool WaveFileData::checkValidWave (std::vector<unsigned char>& fileData)
{
    /*Offset  Size  Name             Description
     * 0         4   ChunkID          Contains the letters "RIFF" in ASCII form
     */
    std::string chunkID (fileData.begin(), fileData.begin() + 4);
    if (chunkID == "RIFF")
        return true;
    else
        return false;

}

bool WaveFileData::openFile ()
{
    std::ifstream infile (_fileName, std::ios::binary);

    if (!infile)
    {
        std::cout<<"\nError opening file : "<<_fileName;
        return false;

    }


    /*
     * When reading characters, std::istream_iterator skips whitespace by default
     * (unless disabled with std::noskipws or equivalent)..
     * http://www.enseignement.polytechnique.fr/informatique/INF478/docs/Cpp/en/cpp/iterator/istream_iterator.html#Notes
     */

    std::noskipws(infile);
    std::istream_iterator<unsigned char> begin (infile), end;
    std::vector<unsigned char> fileData (begin, end);

    if(checkValidWave(fileData))
    {
        _fileData = fileData;
        parse();
        return true;
    }

    else
    {
        std::cout<<"\nInvalid WAV file!";
        return false;
    }

}

bool WaveFileData::parse()
{
    /* Wave file format :

        Offset  Size  Name             Description

        The canonical WAVE format starts with the RIFF header:

        0         4   ChunkID          Contains the letters "RIFF" in ASCII form
                                       (0x52494646 big-endian form).
        4         4   ChunkSize        36 + SubChunk2Size, or more precisely:
                                       4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
                                       This is the size of the rest of the chunk
                                       following this number.  This is the size of the
                                       entire file in bytes minus 8 bytes for the
                                       two fields not included in this count:
                                       ChunkID and ChunkSize.
        8         4   Format           Contains the letters "WAVE"
                                       (0x57415645 big-endian form).

        The "WAVE" format consists of two subchunks: "fmt " and "data":
        The "fmt " subchunk describes the sound data's format:

        12        4   Subchunk1ID      Contains the letters "fmt "
                                       (0x666d7420 big-endian form).
        16        4   Subchunk1Size    16 for PCM.  This is the size of the
                                       rest of the Subchunk which follows this number.
        20        2   AudioFormat      PCM = 1 (i.e. Linear quantization)
                                       Values other than 1 indicate some
                                       form of compression.
        22        2   NumChannels      Mono = 1, Stereo = 2, etc.
        24        4   SampleRate       8000, 44100, etc.
        28        4   ByteRate         == SampleRate * NumChannels * BitsPerSample/8
        32        2   BlockAlign       == NumChannels * BitsPerSample/8
                                       The number of bytes for one sample including
                                       all channels. I wonder what happens when
                                       this number isn't an integer?
        34        2   BitsPerSample    8 bits = 8, 16 bits = 16, etc.
                  2   ExtraParamSize   if PCM, then doesn't exist
                  X   ExtraParams      space for extra parameters

        The "data" subchunk contains the size of the data and the actual sound:

        36        4   Subchunk2ID      Contains the letters "data"
                                       (0x64617461 big-endian form).
        40        4   Subchunk2Size    == NumSamples * NumChannels * BitsPerSample/8
                                       This is the number of bytes in the data.
                                       You can also think of this as the size
                                       of the read of the subchunk following this
                                       number.
        44        *   Data             The actual sound data.

        SOURCE : http://soundfile.sapp.org/doc/WaveFormat/

        NOTE : I later found out that the subChunks may be located at different offsets
               in presence of metadata like LIST INFO etc..

     */

    std::string format(_fileData.begin() + 8, _fileData.begin() + 12);

    if(format != "WAVE")
    {
        std::cout<<"\nInvalid Format : "<<format;
        return false;
    }

    /*
     * Apparently, this is just not it. The `fmt ` and `data`  chunk may not necessarily be in continuation.
     * There may occur inclusion of metadata. So, we'll need to find the location of these chunks.
     */

    int fmtIndex = findIndex(_fileData, "fmt ");
    int dataIndex = findIndex(_fileData, "data");

    std::string subChunk1ID(_fileData.begin() + fmtIndex, _fileData.begin() + fmtIndex + 4);

    if(subChunk1ID != "fmt ")
    {
        std::cout<<"\nInvalid SubChunk1ID : "<<subChunk1ID;
        return false;
    }

    unsigned long subChunk1Size = fourBytesToInt(_fileData, fmtIndex + 4);

    if(subChunk1Size != 16)
    {
        std::cout<<"\nNot PCM, SubChunk1Size : "<<subChunk1Size;
        return false;
    }

    int audioFormat = twoBytesToInt(_fileData, fmtIndex + 8);

    if(audioFormat != 1)
    {
        std::cout<<"\nNot PCM, AudioFormat : "<<audioFormat;
        return false;
    }

    int numChannels = twoBytesToInt(_fileData, fmtIndex + 10);

    if(numChannels != 1)
    {
        std::cout<<"\nNot Mono, NumChannels : "<<numChannels;
        return false;
    }

    unsigned long sampleRate = fourBytesToInt(_fileData, fmtIndex + 12);

    if(sampleRate != 16000)
    {
        std::cout<<"\nNot 16000Hz SampleRate, SampleRate : "<<sampleRate;
        return false;
    }

    unsigned long byteRate = fourBytesToInt(_fileData, fmtIndex + 16);

    int blockAlign = twoBytesToInt(_fileData, fmtIndex + 20);

    int bitRate = twoBytesToInt(_fileData, fmtIndex + 22); //BitsPerSample

    if(bitRate != 16)
    {
        std::cout<<"\nNot 16 bits/sec, BitRate : "<<bitRate;
        return false;
    }

    if((byteRate != sampleRate * numChannels * bitRate/8) || (blockAlign != numChannels * bitRate/8))
    {
        std::cout<<"\nIncorrect header, ByteRate and/or BlockAlign values do not match!";
        return false;
    }

    std::string subChunk2ID (_fileData.begin() + dataIndex, _fileData.begin() + dataIndex + 4);

    if(subChunk2ID != "data")
    {
        std::cout<<"\nInvalid SubChunk2ID : "<<subChunk2ID;
        return false;
    }

    unsigned long subChunk2Size = fourBytesToInt(_fileData, dataIndex + 4);

    int numSamples = subChunk2Size * 8 / ( numChannels * bitRate);

    for (int i = 0; i < numSamples; i++)
    {
            int sampleIndex = dataIndex + 8 + (blockAlign * i) + bitRate / 8;
            // dataIndex = index of beginning of "data" subChunk
            // dataIndex + 8 is usually 44 as per the specs

            int16_t sample = twoBytesToInt(_fileData, sampleIndex);
            _samples.push_back(sample);
    }

    return true;
}

/* Convert 4 bytes to int
 * https://stackoverflow.com/a/2386134/6487831
 */

unsigned long WaveFileData::fourBytesToInt (std::vector<unsigned char>& fileData, int index)
{
    return ((fileData[index + 3] << 24) | (fileData[index + 2] << 16) | (fileData[index + 1] << 8) | fileData[index]);
}

int WaveFileData::twoBytesToInt (std::vector<unsigned char>& fileData, int index)
{
    return ((fileData[index + 1] << 8) | fileData[index]);
}

/* Convert signed int (2bytes, 16 bits) in double format
 * https://stackoverflow.com/a/4619787/6487831
 */
double WaveFileData::twoBytesToDouble (int value)
{
    return (double)value / (double) 32768.0;
}

std::vector<int16_t> WaveFileData::getSamples()
{
    return _samples;
}
