/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "generate_approx_timestamp.h"

int CurrentSub::_wordNumber;    //defining a static data member

CurrentSub::CurrentSub(SubtitleItem *sub) noexcept
    : _sub(sub),
      _sentenceLength(sub->getDialogue().size()),
      _wordCount(sub->getWordCount()),
      _dialogueDuration(getDuration(sub->getStartTime(), sub->getEndTime()))
{}

void CurrentSub::printToSRT(const std::string& fileName, outputOptions printOption) const
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

void CurrentSub::printToConsole(const std::string& fileName) const
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

inline int CurrentSub::getDuration (long startTime, long endTime) const noexcept
{
    if(endTime < startTime)
    {
        std::cout << "Error! Incorrect start time and end time of the dialogue.\n";
        return -1;
    }

    return endTime - startTime;

}

inline double CurrentSub::getWordWeight (const std::string& word) const noexcept
{
    double weight = static_cast<double>(word.size()) / static_cast<double>(_sentenceLength);    //word weight as function of word length : sentence length
    return weight;

}

void CurrentSub::assignTime(long int &wordDuration, const std::string &word )
{
    if(_wordNumber>_wordCount)
        std::cout<<"Oops! Something went wrong!\n";

    double wordWeight = getWordWeight(word);
    wordDuration = wordWeight * _dialogueDuration;
    _wordNumber++;

}

void CurrentSub::run()
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


NonAlignedBlock::NonAlignedBlock() noexcept
    : wordLength(),
      startTime(),
      endTime(),
      startIndex(),
      endIndex()
{}

void CurrentSub::alignNonRecognised(recognisedBlock currBlock)
{
    long int startTime = _sub->getStartTime(), endTime = _sub->getEndTime(), duration = startTime - endTime;


    NonAlignedBlock nonAlignedBlock;
    nonAlignedBlock.startTime = startTime;
    nonAlignedBlock.endTime = startTime;

    for(int i=0;i<_sub->getWordCount();i++)
    {
        if(!_sub->getWordRecognisedStatusByIndex(i))
        {
            nonAlignedBlock.wordLength += _sub->getWordByIndex(i).size();
            nonAlignedBlock.endIndex = i;
        }

        else
        {
            if(nonAlignedBlock.wordLength > 0)
            {
                nonAlignedBlock.endTime = _sub->getWordStartTimeByIndex(i);
                nonAlignedBlock.timespan = nonAlignedBlock.endTime - nonAlignedBlock.startTime;

                for(int j = nonAlignedBlock.startIndex; j <= nonAlignedBlock.endIndex ; j++)
                {
                    int indi = _sub->getWordByIndex(j).size() / nonAlignedBlock.wordLength * nonAlignedBlock.timespan;
                    _sub->setWordTimesByIndex(nonAlignedBlock.startTime, nonAlignedBlock.startTime + indi, j);
                    nonAlignedBlock.startTime = nonAlignedBlock.startTime + indi;
                }
            }

            nonAlignedBlock.startTime = _sub->getWordEndTimeByIndex(i);
            nonAlignedBlock.endTime = nonAlignedBlock.startTime;

            nonAlignedBlock.wordLength = 0;
            nonAlignedBlock.startIndex = i+1;
        }
    }

}

ApproxAligner::ApproxAligner(std::string fileName, outputFormats outputFormat) noexcept
    : _fileName(std::move(fileName)),
      _outputFormat(outputFormat),
      _outputFileName(extractFileName(_fileName))
{}

ApproxAligner::ApproxAligner(Params * parameters) noexcept
    : _parameters(parameters),
      _fileName(parameters->subtitleFileName),
      _outputFormat(parameters->outputFormat),
      _outputFileName(parameters->outputFileName)
{}

std::vector<SubtitleItem *, std::allocator<SubtitleItem *>> ApproxAligner::align()
{
    SubtitleParserFactory subParserFactory(_fileName);
    SubtitleParser *parser = subParserFactory.getParser();
    std::vector <SubtitleItem*> subtitles = parser->getSubtitles();

    int subCount = 1;
    initFile(_outputFileName, _outputFormat);

    for(SubtitleItem *sub : subtitles)
    {
        CurrentSub currSub(sub);
        currSub.run();

        switch (_outputFormat)  //decide on basis of set output format
        {
            case srt:       subCount = printSRTContinuous(_outputFileName, subCount, sub, printBothWihoutColors);
                break;

            case xml:       printXMLContinuous(_outputFileName, sub);
                break;

            case json:      printJSONContinuous(_outputFileName, sub);
                break;

            case karaoke:   subCount = printKaraokeContinuous(_outputFileName, subCount, sub, printBothWihoutColors);
                break;

            case console:   currSub.printToConsole(_outputFileName);
                break;

            default:        std::cout<<"An error occurred while choosing output format!";
                exit(2);
        }

    }

    printFileEnd(_outputFileName, _outputFormat);
    return subtitles;
}
