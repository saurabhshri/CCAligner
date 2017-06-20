/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef GENERATE_APPROX_TIMESTAMP_H
#define GENERATE_APPROX_TIMESTAMP_H

#include "srtparser.h"

enum outputFormats
{
    srt,
    xml,
    json,
    console,
    blank
};

class currentSub
{
    int _sentenceLength, _wordCount;
    long _dialogueDuration;
    static int _wordNumber;
    SubtitleItem *_sub;


public:
    int getDuration (long startTime, long endTime);
    double getWordWeight (std::string word);
    currentSub(SubtitleItem *sub);
    void run();
    void printToSRT(std::string fileName);
    void printToJSON(std::string fileName);
    void printToXML(std::string fileName);
    void printToConsole(std::string fileName);
    void assignTime(long int &wordDuration, const std::string &word );
    ~currentSub();
};

class ApproxAligner
{
private:
    std::string _fileName,_outputFileName;
    outputFormats _outputFormat;
public:
    ApproxAligner(std::string fileName, outputFormats outputFormat = srt);
    void align();
    ~ApproxAligner();
};

void ms_to_srt_time(long int ms, int *hours, int *minutes, int *seconds, int *milliseconds);
std::string extractFileName(std::string fileName);

#endif //GENERATE_APPROX_TIMESTAMP_H
