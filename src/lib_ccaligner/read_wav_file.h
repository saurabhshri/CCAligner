/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_READ_WAV_FILE_H
#define CCALIGNER_READ_WAV_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>

enum openMode
{
    readFile,
    readStreamDirectly,
    readStreamIntoBuffer,

};

int findIndex(std::vector<unsigned char>& fileData, std::string chunk);

class WaveFileData
{
    std::string _fileName;
    std::vector<unsigned char> _fileData;
    std::vector<int16_t> _samples;
    openMode _openMode;

    bool checkValidWave (std::vector<unsigned char>& fileData);
    bool parse();

    int processStreamHeader();
    int seekToEndOfSubChunk1ID(int remainingBytes);
    int validateSubChunk1(int remainingBytes);
    int seekToEndOfSubChunk2ID(int remainingBytes);
    int getNumberOfSamples();
    bool readSamplesFromStream(int numberOfSamples);

    unsigned long fourBytesToInt (std::vector<unsigned char>& fileData, int index);
    int twoBytesToInt (std::vector<unsigned char>& fileData, int index);
    double twoBytesToDouble (int sample);

public:
    WaveFileData(std::string fileName);
    WaveFileData(openMode mode = readStreamDirectly);
    bool openFile();
    bool readStream();
    bool readStreamUsingBuffer();
    bool read();
    std::vector<int16_t> getSamples();
    ~WaveFileData();
};

#endif //CCALIGNER_READ_WAV_FILE_H
