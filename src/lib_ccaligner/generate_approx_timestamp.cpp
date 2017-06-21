/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "generate_approx_timestamp.h"

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

std::string extractFileName(std::string fileName)
{
    int lastIndex = fileName.find_last_of(".");

    if(lastIndex == std::string::npos)
        return fileName;
    else
        return fileName.substr(0, lastIndex);
}

int currentSub::_wordNumber;

currentSub::currentSub(SubtitleItem *sub)
{
    _sub = sub;
    _wordNumber = 0;
    _sentenceLength = _sub->getDialogue().size();
    _wordCount = _sub->getWordCount();
    _dialogueDuration = getDuration(_sub->getStartTime(), _sub->getEndTime());

}


void currentSub::printToSRT(std::string fileName)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::app);

    for(int i=0;i<_sub->getWordCount();i++)
    {
        int hh1,mm1,ss1,ms1;
        int hh2,mm2,ss2,ms2;
        char timeline[128];

        ms_to_srt_time(_sub->getWordStartTimeByIndex(i),&hh1,&mm1,&ss1,&ms1);
        ms_to_srt_time(_sub->getWordEndTimeByIndex(i),&hh2,&mm2,&ss2,&ms2);

        sprintf(timeline, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\n", hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        out<<timeline;
        out<<_sub->getWordByIndex(i)<<"\n\n";
    }
}

void currentSub::printToXML(std::string fileName)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::app);

    out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE subtitle>\n\n";

    /*
     * ├───Subtitle
     *      ├───Start Time
     *      ├───End Time
     *      ├───Start Time in ms
     *      ├───Start Time in ms
     *      ├───Word
     *      ├───Style
     *      |   ├───Style Tag 1
     *      |   ├───Style Tag 2
     *      |   └───Style Tag
     */

    for(int i=0;i<_sub->getWordCount();i++)
    {
        out<<"<subtitle>\n";
        int hh1,mm1,ss1,ms1;
        int hh2,mm2,ss2,ms2;
        char printLine[150];

        ms_to_srt_time(_sub->getWordStartTimeByIndex(i),&hh1,&mm1,&ss1,&ms1);
        ms_to_srt_time(_sub->getWordEndTimeByIndex(i),&hh2,&mm2,&ss2,&ms2);

        sprintf(printLine, "\t<time start=%ld end=%ld></time>\n", _sub->getWordStartTimeByIndex(i), _sub->getWordEndTimeByIndex(i));
        out<<printLine;
        sprintf(printLine, "\t<srtTime start=%02d:%02d:%02d,%03d end=%02d:%02d:%02d,%03d></srtTime>\n", hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);
        out<<printLine;
        out<<"\t<text>";
        out<<_sub->getWordByIndex(i)<<"</text>\n";
        out<<"</subtitle>\n\n";
    }
}

void currentSub::printToJSON(std::string fileName)
{
    //make JSON as output
    std::cout<<"JSON format is yet to be decided :) Try xml/srt!\n";
    exit(2);
}

void currentSub::printToConsole(std::string fileName)
{
    for(int i=0;i<_sub->getWordCount();i++)
    {
        std::cout<<"START\n";
        std::cout<<"word    : "<<_sub->getWordByIndex(i)<<"\n";
        std::cout<<"start   : "<<_sub->getWordStartTimeByIndex(i)<<" ms\n";
        std::cout<<"end     : "<<_sub->getWordEndTimeByIndex(i)<<" ms\n";
        std::cout<<"END\n\n";
    }
}

inline int currentSub::getDuration (long startTime, long endTime)
{
    if(endTime < startTime)
    {
        std::cout << "Error! Incorrect start time and end time of the dialogue.\n";
        return -1;
    }

    return endTime - startTime;

}

inline double currentSub::getWordWeight (std::string word)
{
    double weight = (double) word.size() / (double) _sentenceLength;
    return weight;

}

void currentSub::assignTime(long int &wordDuration, const std::string &word )
{
    if(_wordNumber>_wordCount)
        std::cout<<"Oops! Something went wrong!\n";

    double wordWeight = getWordWeight(word);
    wordDuration = wordWeight * _dialogueDuration;
    _wordNumber++;

}

void currentSub::run()
{
    std::vector<std::string> words = _sub->getIndividualWords();
    std::vector<long int> wordDuration(_wordCount), wordStartTime(_wordCount), wordEndTime(_wordCount);
    int totalWordDuration = 0;

    for(int i = 0; i < _wordCount; i++)
    {
        assignTime(wordDuration[i], words[i]);
        totalWordDuration+=wordDuration[i];
    }

    int silenceDuration = (_wordCount!=1)?((_dialogueDuration - totalWordDuration) / (_wordCount - 1)):0;
    long int prevTime = _sub->getStartTime();


    for(int i = 0; i < _wordCount; i++)
    {
        if(i == _wordCount - 1)
            silenceDuration = 0;

        wordStartTime[i] = prevTime;
        wordEndTime[i] = prevTime + silenceDuration + wordDuration[i];
        prevTime = wordEndTime[i];
    }

    _sub->setWordTimes(wordStartTime, wordEndTime, wordDuration);
}

currentSub::~currentSub()
{
    _sub = NULL;
    _wordNumber = 0;
    _sentenceLength = 0;
    _wordCount = 0;
    _dialogueDuration = 0;
}

ApproxAligner::ApproxAligner(std::string fileName, outputFormats outputFormat)
{
    _fileName = fileName;
    _outputFormat = outputFormat;
    _outputFileName = extractFileName(_fileName);

    std::ofstream out;
    out.open(_outputFileName);
    out.close();
}

void ApproxAligner::align()
{
    SubtitleParserFactory *subParserFactory = new SubtitleParserFactory(_fileName);
    SubtitleParser *parser = subParserFactory->getParser();
    std::vector <SubtitleItem*> subtitles = parser->getSubtitles();

    for(SubtitleItem *sub : subtitles)
    {
        currentSub * currSub = new currentSub(sub);
        currSub->run();

        switch (_outputFormat)
        {
            case srt:   currSub->printToSRT(_outputFileName);
                        break;

            case xml:   currSub->printToXML(_outputFileName);
                        break;

            case json:  currSub->printToJSON(_outputFileName);
                        break;

            case console: currSub->printToConsole(_outputFileName);
                        break;

            default: std::cout<<"An error occurred while choosing output format!";
                        exit(2);
        }

        delete currSub;
    }

    delete parser;
    delete subParserFactory;
}

ApproxAligner::~ApproxAligner()
{


}
