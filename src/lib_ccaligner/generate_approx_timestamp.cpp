/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "generate_approx_timestamp.h"

int currentSub::_wordNumber;    //defining static data member

currentSub::currentSub(SubtitleItem *sub)
{
    _sub = sub;
    _wordNumber = 0;
    _sentenceLength = _sub->getDialogue().size();   //length of complete dialogue in current subtitle
    _wordCount = _sub->getWordCount();              //no. of words in current subtitle
    _dialogueDuration = getDuration(_sub->getStartTime(), _sub->getEndTime());

}

void currentSub::printToSRT(std::string fileName, outputOptions printOption)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::app);

    for(int i=0;i<_sub->getWordCount();i++)
    {
        if(printOption == printOnlyRecognised)
        {
            if(!_sub->getWordRecognisedStatusByIndex(i))
                continue;

        }
        int hh1,mm1,ss1,ms1;
        int hh2,mm2,ss2,ms2;
        char timeline[128];

        ms_to_srt_time(_sub->getWordStartTimeByIndex(i),&hh1,&mm1,&ss1,&ms1);
        ms_to_srt_time(_sub->getWordEndTimeByIndex(i),&hh2,&mm2,&ss2,&ms2);

        //printing in SRT format
        sprintf(timeline, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\n", hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        out<<_sub->getSubNo()<<"\n";
        out<<timeline;

        if(printOption == printBothWithDistinctColors)
        {
            if(!_sub->getWordRecognisedStatusByIndex(i))
                out<<"<font color='#FF0000'>"<<_sub->getWordByIndex(i)<<"</font>"<<"\n\n";
            else
                out<<_sub->getWordByIndex(i)<<"\n\n";
        }
        else
            out<<_sub->getWordByIndex(i)<<"\n\n";
    }

    out.close();
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
    double weight = (double) word.size() / (double) _sentenceLength;    //word weight as function of word length : sentence length
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

    //compensating silence
    int silenceDuration = (_wordCount!=1)?((_dialogueDuration - totalWordDuration) / (_wordCount - 1)):0;
    long int prevTime = _sub->getStartTime();


    for(int i = 0; i < _wordCount; i++)
    {
        if(i == _wordCount - 1)
            silenceDuration = 0;

        wordStartTime[i] = prevTime;
        wordEndTime[i] = prevTime + silenceDuration + wordDuration[i]; //assigning correct timestamp
        prevTime = wordEndTime[i];
    }

    _sub->setWordTimes(wordStartTime, wordEndTime, wordDuration);
}

void currentSub::alignNonRecognised(recognisedBlock currBlock)  //TODO
{
    long int startTime = _sub->getStartTime(), endTime = _sub->getEndTime(), duration = startTime - endTime;

    long int breakPoints [_sub->getWordCount() * 2 + 1][3];   //suppose there are silence after each word + start and end silence

    for(int r = 0; r < _sub->getWordCount() * 2 + 1; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            breakPoints[r][c] = 0;      //init with 0
        }
    }
    //trimming starting and ending silences

    if(currBlock.recognisedString[0] == "<s>")
    {
        startTime += currBlock.recognisedWordStartTimes[0];
    }

    if(currBlock.recognisedString[currBlock.recognisedString.size()] == "</s>")
    {
        endTime -= currBlock.recognisedWordEndTimes[currBlock.recognisedString.size()];
    }

    //creating breakpoints in the timeline for recognised words and silences

    long int breakPointCounter = 0;

    for(int i=0;i<_sub->getWordCount();i++)
    {
        if(_sub->getWordRecognisedStatusByIndex(i))
        {
            breakPoints[breakPointCounter][0] = _sub->getWordStartTimeByIndex(i);
            breakPoints[breakPointCounter][1] = _sub->getWordEndTimeByIndex(i);
            breakPoints[breakPointCounter][2] = i;
            breakPointCounter++;
        }
    }

    for(int i=0;i<currBlock.recognisedString.size();i++)
    {
        if(currBlock.recognisedString[i] == "<sil>")
        {
            breakPoints[breakPointCounter][0] = currBlock.recognisedWordStartTimes[i];
            breakPoints[breakPointCounter][1] =  currBlock.recognisedWordEndTimes[i];
            breakPoints[breakPointCounter][2] = -1;
            breakPointCounter++;
        }
    }

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
}

ApproxAligner::ApproxAligner(Params * parameters)
{
    _parameters = parameters;
    _fileName = _parameters->subtitleFileName;
    _outputFormat = _parameters->outputFormat;
    _outputFileName = _parameters->outputFileName;
}

std::vector<SubtitleItem *, std::allocator<SubtitleItem *>> ApproxAligner::align()
{
    SubtitleParserFactory *subParserFactory = new SubtitleParserFactory(_fileName);
    SubtitleParser *parser = subParserFactory->getParser();
    std::vector <SubtitleItem*> subtitles = parser->getSubtitles();

    int subCount = 1;
    initFile(_outputFileName, _outputFormat);

    for(SubtitleItem *sub : subtitles)
    {
        currentSub * currSub = new currentSub(sub);
        currSub->run();

        switch (_outputFormat)  //decide on based of set output format
        {
            case srt:       subCount = printSRTContinuous(_outputFileName, subCount, sub, printBothWihoutColors);
                break;

            case xml:       printXMLContinuous(_outputFileName, sub);
                break;

            case json:      printJSONContinuous(_outputFileName, sub);
                break;

            case karaoke:   subCount = printKaraokeContinuous(_outputFileName, subCount, sub, printBothWihoutColors);
                break;

            case console:   currSub->printToConsole(_outputFileName);
                break;

            default:        std::cout<<"An error occurred while choosing output format!";
                exit(2);
        }

    }

    printFileEnd(_outputFileName, _outputFormat);
    return subtitles;
}

ApproxAligner::~ApproxAligner()
{

}
