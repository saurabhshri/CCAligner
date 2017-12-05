/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "commons.h"

Logger log;

Logger::Output::Output(std::ostream * stream, Level::LevelFlag level) noexcept
    : _stream(stream)
{
    if (!_stream)
        return;

    std::string levelString;
    switch (level) {
    case Level::Debug: levelString = "Debug";
        break;
    case Level::Info: break;
    case Level::Warning: levelString = "Warning";
        break;
    case Level::Fatal: levelString = "Fatal";
        break;
    default:
        assert(!"Could not convert log level to string.");
    }

    if (!levelString.empty())
        *_stream << '[' << levelString << "] ";
}

Logger::Output::~Output()
{
    if (_stream)
        *_stream << std::endl;
}

auto Logger::operator()(Level::LevelFlag level) const noexcept -> Output
{
    return Output(level & _filter ? nullptr : _stream, level);
}

void Logger::turnOff() noexcept
{
    _stream = nullptr;
}

void Logger::filter(Level::LevelFlag flags) noexcept
{
    _filter = flags;
}


void Logger::writeToFile(const std::string& filename) noexcept
{
    _file.open(filename);
    _stream = &_file;
}

void Logger::writeToStdout() noexcept
{
    _stream = &std::cout;
}

void Logger::writeToStderr() noexcept
{
    _stream = &std::cerr;
}

void Logger::writeToStream(std::ostream& stream) noexcept
{
    _stream = &stream;
}

void ms_to_srt_time(long int ms, int *hours, int *minutes, int *seconds, int *milliseconds)
{
    *milliseconds = (int)(ms % 1000);
    ms = (ms - *milliseconds) / 1000;

    *seconds = (int)(ms % 60);
    ms = (ms - *seconds) / 60;

    *minutes = (int)(ms % 60);
    ms = (ms - *minutes) / 60;

    *hours = (int)ms;
}

std::string extractFileName(const std::string& fileName)
{
    int lastIndex = fileName.find_last_of(".");

    if(lastIndex == std::string::npos)
        return fileName;    //if no extension is present, return complete filename
    else
        return fileName.substr(0, lastIndex);
}

std::string stringToLower(std::string strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::tolower);

    return strToConvert;
}

bool AlignedData::addNewWord(const std::string& word, long int startTime, long int endTime, float conf)
{
    _words.push_back(word);
    _wordStartTimes.push_back(startTime);
    _wordEndTimes.push_back(endTime);
    _wordConf.push_back(conf);

    return true;
}

file_not_found_exception::file_not_found_exception(std::string file) noexcept
    : std::runtime_error(std::move(file))
{}

invalid_file_exception::invalid_file_exception(std::string file) noexcept
    : std::runtime_error(std::move(file))
{}

incomplete_parameters_exception::incomplete_parameters_exception(std::string problem) noexcept
    : std::invalid_argument(std::move(problem))
{}

incompatible_parameters_exception::incompatible_parameters_exception(std::string problem) noexcept
    : std::invalid_argument(std::move(problem))
{}
