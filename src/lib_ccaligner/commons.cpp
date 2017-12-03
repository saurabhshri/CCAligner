/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "commons.h"

bool should_log;

/* Write formatted message to stderr and then exit. */
void fatal(int exit_code, const char *fileName, const int lineNumber, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    fprintf(stderr, "\r\n\n\n[ERROR] %s : %d \n\t\t ", fileName, lineNumber);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n\n\n");
    va_end(args);
    std::exit(exit_code);
}


/* Shorten some debug output code. */
void log(const char *fileName, const int lineNumber, const char *format, ...)
{
    if(should_log == false)
        return;

    va_list args;
    va_start(args, format);
    fprintf(stdout, "[INFO] %s : %d | ", fileName, lineNumber);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    fflush (stdout);
    va_end(args);
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
