/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "generate_approx_timestamp.h"
#include <fstream>


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

int currentSub::wordNumber;

currentSub::currentSub(SubtitleItem *sub)
{
    _sub = sub;
    wordNumber = 0;
    sentenceLength = _sub->getDialogue().size();
    wordCount = _sub->getWordCount();
    dialogueDuration = getDuration(_sub->getStartTime(), _sub->getEndTime());

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
    double weight = (double) word.size() / (double) sentenceLength;
    return weight;

}

void currentSub::assignTime(long int &wordDuration, const std::string &word )
{
    if(wordNumber>wordCount)
        std::cout<<"Oops! Something went wrong!\n";

    double wordWeight = getWordWeight(word);
    wordDuration = wordWeight * dialogueDuration;
    wordNumber++;

}

void currentSub::printToSrt(std::string fileName)
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


void currentSub::run()
{
    std::vector<std::string> words = _sub->getIndividualWords();
    std::vector<long int> wordDuration(wordCount), wordStartTime(wordCount), wordEndTime(wordCount);
    int totalWordDuration = 0;

    for(int i = 0; i < wordCount; i++)
    {
        assignTime(wordDuration[i], words[i]);
        totalWordDuration+=wordDuration[i];
    }

    int silenceDuration = (wordCount!=1)?((dialogueDuration - totalWordDuration) / (wordCount - 1)):0;
    long int prevTime = _sub->getStartTime();


    for(int i = 0; i < wordCount; i++)
    {
        if(i == wordCount - 1)
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
    wordNumber = 0;
    sentenceLength = 0;
    wordCount = 0;
    dialogueDuration = 0;
}

ApproxAligner::ApproxAligner(std::string fileName)
{
    _fileName = fileName;
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
        currSub->printToSrt("ElonMusk.srt");
        delete currSub;
    }

    delete parser;
    delete subParserFactory;
}

ApproxAligner::~ApproxAligner()
{


}
