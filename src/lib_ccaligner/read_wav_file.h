/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_READ_WAV_FILE_H
#define CCALIGNER_READ_WAV_FILE_H

#include "commons.h"
#include "params.h"

enum openMode
{
    readFile,               //read from a file located on disk
    readStreamDirectly,     //read directly from stream/piped data
    readStreamIntoBuffer,   //read data from stream/pipe into buffer first, then process

};

int findIndex(std::vector<unsigned char>& fileData, const std::string& chunk); //returns the index of beginning of the "chunk" string

class WaveFileData
{
    std::string _fileName;                  //name/path of the wave file
    std::vector<unsigned char> _fileData;   //content of the wave file
    std::vector<int16_t> _samples;          //the raw samples containing audio data : PCM, 16 bit, Sampled at 16Khz, mono
    openMode _openMode;                     //mode of reading file
    bool _isRawFile;                        //if the audio is raw audio file

    //when reading from file or buffer
    bool checkValidWave (const std::vector<unsigned char>& fileData); //check if wave file is valid by reading the RIFF header
    bool decode();  //decode the wave file and fill the _sample vector

    //when reading from stream or pipe
    int processStreamHeader();                      //check if stream is valid wave stream
    int seekToEndOfSubChunk1ID(int remainingBytes); //reach till the end of 'fmt' Chunk ID + skip 1 byte (empty byte)
    int validateSubChunk1(int remainingBytes);      //check the 'fmt' chunk for required wave file config (16 bit, 16KHz, mono, PCM)
    int seekToEndOfSubChunk2ID(int remainingBytes); //reach till the end of 'data' Chunk ID
    int getNumberOfSamples();                       //basically gets size of 'data' Chunk which contains size of samples
    bool readSamplesFromStream(int numberOfSamples);//read the sample from stream and insert in the _sample vector

    unsigned long fourBytesToInt (const std::vector<unsigned char>& fileData, int index); //convert 4 bytes into unsigned long int
    int twoBytesToInt (const std::vector<unsigned char>& fileData, int index);            //convert 2 bytes into signed integer
    double twoBytesToDouble (int sample);                                           //convert 2 bytes to double; not required rn

public:
    WaveFileData(std::string fileName, bool isRawFile = false) noexcept;                    //initialize wave file for file on disk mode; pass file name
    WaveFileData(openMode mode = readStreamDirectly, bool isRawFile = false) noexcept;   //initialize wave file for stream mode; optionally store in buffer

    bool openFile();                //open file using file name
    bool readStream();              //process stream directly
    bool readStreamUsingBuffer();   //first store stream into buffer, then process
    bool read();                    //the main function which decides the open method using set mode

    const std::vector<int16_t>& getSamples() const noexcept;  //returns the sample vector; time based coming soon
};

#endif //CCALIGNER_READ_WAV_FILE_H
