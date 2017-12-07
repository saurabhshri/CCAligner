/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_UTILITY_H
#define CCALIGNER_UTILITY_H

#include <iostream>
#include <stdexcept>
#include <system_error>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <regex>
#include <cstdarg>
#include <memory>
#include "logger.h"

enum alignerType
{
    approxAligner,          //approximation based alignment
    asrAligner,             //using pocketsphinx as ASR
    alignerUnassigned       //no aligner is specified
};

enum outputFormats      //various possible output formats
{
    srt,
    xml,
    json,
    karaoke,
    console,
    blank               //means no output format is specified
};

enum grammarName      //various possible output formats
{
    corpus,
    fsg,
    lm,
    phone_lm,
    dict,
    vocab,
    complete_grammar,
    quick_dict,
    quick_lm,
    no_grammar
};

enum outputOptions
{
    printOnlyRecognised,
    printBothWithoutColors,
    printBothWithDistinctColors,
    printAsKaraoke,
    printAsKaraokeWithDistinctColors,

};

void ms_to_srt_time(long int ms, int *hours, int *minutes, int *seconds, int *milliseconds); //converts ms to SRT time
std::string extractFileName(const std::string& fileName);  //extract path/to/filename from path/to/filename.extension
std::string stringToLower(std::string strToConvert);

class AlignedData
{
public:

    std::vector<std::string> _words;
    std::vector<long int>   _wordStartTimes;
    std::vector<long int>   _wordEndTimes;
    std::vector<float>      _wordConf;

    bool addNewWord(const std::string& word, long int startTime, long int endTime, float conf);

    std::vector<std::string> _phonemes;
    std::vector<long int>   _phonemeStartTimes;
    std::vector<long int>   _phonemeEndTimes;
    std::vector<float>      _phonemeConf;

    bool addNewPhoneme(const std::string& phoneme, long int startTime, long int endTime, float conf);

};


#endif //CCALIGNER_UTILITY_H
