/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "generate_approx_timestamp.h"

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
    //dialogueDuration -= wordDuration;
    wordNumber++;

}


void currentSub::run()
{
    std::vector<std::string> words = _sub->getIndividualWords();
    std::vector<long int> wordDuration(wordCount);
    int time = 0;

    for(int i = 0; i < wordCount; i++)
    {
        assignTime(wordDuration[i], words[i]);
        std::cout<<"Word = "<<words[i]<<std::endl;
        std::cout<<"Total Sentence Duration = "<<dialogueDuration<<std::endl;
        std::cout<<"Estimated Time Duration of Word = "<<wordDuration[i]<<std::endl<<std::endl;
        time+=wordDuration[i];

    }

    std::cout<<"Are they equal ? "<<dialogueDuration<< " == "<<time<<std::endl;
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
    }
}

ApproxAligner::~ApproxAligner()
{


}
