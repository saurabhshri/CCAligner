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

extern bool should_log;

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
    printBothWihoutColors,
    printBothWithDistinctColors,
    printAsKaraoke,
    printAsKaraokeWithDistinctColors,

};

// Define possible exit codes that will be passed on to the fatal function exit codes.
#define EXIT_OK                                 0
#define EXIT_FILE_NOT_FOUND                     2
#define EXIT_INVALID_PARAMETERS                 3
#define EXIT_INCOMPATIBLE_PARAMETERS            4
#define EXIT_INCOMPLETE_PARAMETERS              5
#define EXIT_INVALID_FILE                       6
#define EXIT_WITH_HELP                          10
#define EXIT_UNKNOWN                            13

#define LOG(MSG, ...) log(__FILE__, __LINE__, MSG, ##__VA_ARGS__)
void log(const char *fileName, const int lineNumber, const char *format, ...);

#define FATAL(ERR_NO, MSG, ...) fatal(ERR_NO, __FILE__, __LINE__, MSG, ##__VA_ARGS__)
void fatal(int exit_code, const char *fileName, const int lineNumber, const char *format,  ...);

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
