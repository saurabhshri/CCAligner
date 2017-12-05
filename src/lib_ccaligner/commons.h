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
#include <ostream>
#include <fstream>
#include <cassert>
#include <stdexcept>
#include <sstream>

namespace Level {
enum LevelFlag
{
    None = 0x0, // dummy flag to output nothing
    Info = 0x1, // useful information that the user should know
    Debug = 0x2, // useful information when the API fails for further investigation
    Warning = 0x4, // information about a behavior of this API which might not be intended
    Fatal = 0x8 // API cannot recover from this (but others might be able to)
};
}

extern class Logger
{
    class Output
    {
        std::ostream * _stream;
    public:
        Output(std::ostream * stream, Level::LevelFlag level) noexcept;
        ~Output();

        template<typename T>
        Output& operator<<(const T& variable) noexcept;
    };

    std::ostream * _stream = &std::cout;
    std::ofstream _file;
    Level::LevelFlag _filter = Level::None;
public:
    void turnOff() noexcept;
    void filter(Level::LevelFlag flags) noexcept;

    void writeToFile(const std::string& filename) noexcept;
    void writeToStdout() noexcept;
    void writeToStderr() noexcept;
    void writeToStream(std::ostream& stream) noexcept;

    Output operator()(Level::LevelFlag level = Level::Debug) const noexcept;

    Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
} Log;

template<typename T>
class FatalOutput
{
    std::stringstream _stream;
public:
    // Yes, the destructor needs to throw an exception.
    // This is safe, because throwing *another* exception calls std::terminate,
    // and fatally aborting the program is the correct action because
    // it is a programmer error.
    ~FatalOutput() noexcept(false);

    template<typename U>
    FatalOutput& operator<<(const U& variable) noexcept;

    FatalOutput() = default;
    FatalOutput(FatalOutput&&) = default;
    FatalOutput& operator=(FatalOutput&&) = default;
};

template<typename T>
FatalOutput<T> Fatal()
{
    return FatalOutput<T>();
}

template<typename T>
Logger::Output& Logger::Output::operator<<(const T& variable) noexcept
{
    if (_stream)
        *_stream << variable;
    return *this;
}

template<typename T>
FatalOutput<T>::~FatalOutput() noexcept(false)
{
    std::string message = _stream.str();
    Log(Level::Fatal) << message;
    throw T(std::move(message));
}

template<typename T>
template<typename U>
FatalOutput<T>& FatalOutput<T>::operator<<(const U& variable) noexcept
{
    _stream << variable;
    return *this;
}

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

struct file_not_found_exception : std::runtime_error
{
    file_not_found_exception(std::string file) noexcept;
};

struct invalid_file_exception : std::runtime_error
{
    invalid_file_exception(std::string file) noexcept;
};


struct incomplete_parameters_exception : std::invalid_argument
{
    incomplete_parameters_exception(std::string problem) noexcept;
};

struct incompatible_parameters_exception : std::invalid_argument
{
    incompatible_parameters_exception(std::string problem) noexcept;
};

#endif //CCALIGNER_UTILITY_H
