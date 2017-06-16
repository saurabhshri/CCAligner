/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef GENERATE_APPROX_TIMESTAMP_H
#define GENERATE_APPROX_TIMESTAMP_H

#include "srtparser.h"

class currentSub
{
    int sentenceLength, wordCount;
    long dialogueDuration;
    static int wordNumber;
    SubtitleItem *_sub;


public:
    int getDuration (long startTime, long endTime);
    double getWordWeight (std::string word);
    currentSub(SubtitleItem *sub);
    void run();
    void printToSrt(std::string fileName);
    void assignTime(long int &wordDuration, const std::string &word );
    ~currentSub();
};

class ApproxAligner
{
private:
    std::string _fileName;
public:
    ApproxAligner(std::string fileName);
    void align();
    ~ApproxAligner();
};

#endif //GENERATE_APPROX_TIMESTAMP_H
