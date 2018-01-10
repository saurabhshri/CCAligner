/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "commons.h"
#include <string>
using namespace std;

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

    string extractFileName(const string &fileName);

{
    int lastIndex = fileName.find_last_of(".");

    if(lastIndex == string npos)
        
        return fileName;    //If no extension is present, return complete filename
    else
        return fileName.substr(0, lastIndex);
}

    string stringToLower(string strToConvert);

{
    transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), tolower);

    return strToConvert;
}

bool AlignedData_addNewWord(const string &word, long int startTime, long int endTime, float conf)
{
    _words.push_back(word);
    _wordStartTimes.push_back(startTime);
    _wordEndTimes.push_back(endTime);
    _wordConf.push_back(conf);

    return true;
}
