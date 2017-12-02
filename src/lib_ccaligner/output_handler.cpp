/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 */
#include "output_handler.h"

bool initFile(const std::string& fileName, outputFormats outputFormat)
{

    std::ofstream out;
    out.open(fileName, std::ofstream::binary);

    if(outputFormat == xml)
    {
        out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
            "<subtitles>\r\n";
    }

    else if(outputFormat == json)
    {
        out<<"{\r\n";
        out<<"\t\"subtitles\": [\r\n";
    }

    out.close();
    return true;
}

bool printFileEnd(const std::string& fileName, outputFormats outputFormat)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    if(outputFormat == xml)
    {
        out<<"</subtitles>\r\n";
    }

    else if(outputFormat == json)
    {
        out<<"\t]\r\n";
        out<<"}\r\n";
    }

    out.close();
    return true;
}

bool printTranscriptionHeader(const std::string& fileName, outputFormats outputFormat)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary);

    if(outputFormat == xml)
    {
        out<<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
            "<words>\r\n";
    }

    else if(outputFormat == json)
    {
        out<<"{\r\n";
        out<<"\t\"words\" : [\r\n";
    }

    out.close();
    return true;
}

bool printTranscriptionFooter(const std::string& fileName, outputFormats outputFormat)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    if(outputFormat == xml)
    {
        out<<"</words>\r\n";
    }

    else if(outputFormat == json)
    {
        out<<"\t]\r\n";
        out<<"}\r\n";

    }

    out.close();
    return true;

}

bool printSRT(const std::string& fileName, std::vector<SubtitleItem *> subtitles, outputOptions printOption)
{
    initFile(fileName, srt);

    int subCount = 1;

    for(SubtitleItem *sub : subtitles)
    {
        for(int i=0;i<sub->getWordCount();i++)
        {
            subCount = printSRTContinuous(fileName, subCount, sub, printOption);
        }
    }

    return true;
}


int printSRTContinuous(const std::string& fileName, int subCount, SubtitleItem *sub, outputOptions printOption)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    for(int i=0;i<sub->getWordCount();i++)
    {
        if(printOption == printOnlyRecognised)
        {
            if(!sub->getWordRecognisedStatusByIndex(i))
                continue;
        }

        int hh1,mm1,ss1,ms1;
        int hh2,mm2,ss2,ms2;
        char timeline[128];

        ms_to_srt_time(sub->getWordStartTimeByIndex(i),&hh1,&mm1,&ss1,&ms1);
        ms_to_srt_time(sub->getWordEndTimeByIndex(i),&hh2,&mm2,&ss2,&ms2);

        //printing in SRT format
        sprintf(timeline, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\n", hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        out<<subCount++<<"\n";
        out<<timeline;

        if(printOption == printBothWithDistinctColors)
        {
            if(!sub->getWordRecognisedStatusByIndex(i))
                out<<"<font color='#FF0000'>"<<sub->getWordByIndex(i)<<"</font>"<<"\n\n";
            else
                out<<sub->getWordByIndex(i)<<"\n\n";
        }
        else
            out<<sub->getWordByIndex(i)<<"\n\n";
    }

    out.close();
    return subCount;
}

int printTranscriptionAsSRTContinuous(const std::string& fileName, AlignedData *alignedData, int printedTillIndex)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    std::string outputString;

    for(int i = printedTillIndex; i < alignedData->_words.size(); i++)
    {
        if (alignedData->_wordConf[i] <= 0.7)
        {
            outputString = "<font color='#FF0000'>";
            outputString += alignedData->_words[i];
            outputString += "</font>\n\n";
        }

        else
        {
            outputString = alignedData->_words[i];
            outputString += "\n\n";
        }

        int hh1, mm1, ss1, ms1;
        int hh2, mm2, ss2, ms2;
        char timeline[128];

        ms_to_srt_time(alignedData->_wordStartTimes[i], &hh1, &mm1, &ss1, &ms1);
        ms_to_srt_time(alignedData->_wordEndTimes[i], &hh2, &mm2, &ss2, &ms2);

        //printing in SRT format
        sprintf(timeline, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\n", hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        out<<i<<"\n";
        out << timeline;
        out << outputString;
    }


    return 0;
}

bool printXML(const std::string& fileName, std::vector<SubtitleItem *> subtitles)
{
    initFile(fileName, xml);

    for(SubtitleItem *sub : subtitles)
    {
        printXMLContinuous(fileName, sub);
    }

    printFileEnd(fileName, xml);
    return true;
}

bool printXMLContinuous(const std::string& fileName, SubtitleItem *sub)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    out<<"\t<subtitle>\r\n";
    out<<"\t\t<start>"<<sub->getStartTime()<<"</start>\r\n";
    out<<"\t\t<dialogue>"<<sub->getText()<<"</dialogue>\r\n";
    out<<"\t\t<edited_dialogue>"<<sub->getDialogue()<<"</edited_dialogue>\r\n";
    out<<"\t\t\t<words>\r\n";

    for(int i=0;i<sub->getWordCount();i++)
    {
        out<<"\t\t\t\t<word>\r\n";
        out<<"\t\t\t\t\t<recognised>"<<sub->getWordRecognisedStatusByIndex(i)<<"</recognised>\r\n";
        out<<"\t\t\t\t\t<text>"<<sub->getWordByIndex(i)<<"</text>\r\n";
        out<<"\t\t\t\t\t<start>"<<sub->getWordStartTimeByIndex(i)<<"</start>\r\n";
        out<<"\t\t\t\t\t<end>"<<sub->getWordEndTimeByIndex(i)<<"</end>\r\n";
        out<<"\t\t\t\t\t<duration>"<<sub->getWordEndTimeByIndex(i) - sub->getWordStartTimeByIndex(i)<<"</duration>\r\n";
        out<<"\t\t\t\t</word>\r\n";

    }
    out<<"\t\t\t</words>\r\n";

    out<<"\t\t\t<phonemes>\r\n";
    for(int i=0; i<sub->getPhonemeCount();i++)
    {
        out<<"\t\t\t\t<phoneme>\r\n";
        out<<"\t\t\t\t\t<text>"<<sub->getPhonemeByIndex(i)<<"</text>\r\n";
        out<<"\t\t\t\t\t<start>"<<sub->getPhonemeStartTimeByIndex(i)<<"</start>\r\n";
        out<<"\t\t\t\t\t<end>"<<sub->getPhonemeEndTimeByIndex(i)<<"</end>\r\n";
        out<<"\t\t\t\t\t<duration>"<<sub->getPhonemeEndTimeByIndex(i) - sub->getPhonemeStartTimeByIndex(i)<<"</duration>\r\n";
        out<<"\t\t\t\t</phoneme>\r\n";
    }
    out<<"\t\t\t</phonemes>\r\n";

    out<<"\t\t<end>"<<sub->getEndTime()<<"</end>\r\n";
    out<<"\t</subtitle>\r\n";

    out.close();
    return true;
}

bool printTranscriptionAsXMLContinuous(const std::string& fileName, AlignedData *alignedData, int printedTillIndex)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    for(int i=printedTillIndex;i<alignedData->_words.size();i++)
    {
        out << "\t<word>\r\n";
        out << "\t\t<text>" << alignedData->_words[i] << "</text>\r\n";
        out << "\t\t<confidence>" << alignedData->_wordConf[i] << "</confidence>\r\n";
        out << "\t\t<start>" << alignedData->_wordStartTimes[i] << "</start>\r\n";
        out << "\t\t<end>" << alignedData->_wordEndTimes[i] << "</end>\r\n";
        out << "\t\t<duration>" << alignedData->_wordEndTimes[i] - alignedData->_wordStartTimes[i] << "</duration>\r\n";
        out << "\t</word>\r\n";
    }

    out.close();
    return true;
}

bool printJSON(const std::string& fileName, std::vector<SubtitleItem *> subtitles)
{
    initFile(fileName, json);

    for(SubtitleItem *sub : subtitles)
    {
        printJSONContinuous(fileName, sub);
    }

    printFileEnd(fileName, json);

    return true;
}

bool printJSONContinuous(const std::string& fileName, SubtitleItem *sub)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    out<<"\t{\r\n";

    out<<"\t\t\"subtitle\" : \""<<sub->getText()<<"\",\r\n";
    out<<"\t\t\"edited_text\" : \""<<sub->getDialogue()<<"\",\r\n";
    out<<"\t\t\"start\" : \""<<sub->getStartTime()<<"\",\r\n";
    out<<"\t\t\"end\" : \""<<sub->getEndTime()<<"\",\r\n";

    out<<"\t\t\"words\" : [\r\n";

    for(int i=0;i<sub->getWordCount();i++)
    {
        out<<"\t\t\t{\r\n";
        out<<"\t\t\t\t\"word\" : \""<<sub->getWordByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"recognised\" : \""<<sub->getWordRecognisedStatusByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"start\" : \""<<sub->getWordStartTimeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"end\" : \""<<sub->getWordEndTimeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"duration\" : \""<<sub->getWordEndTimeByIndex(i) - sub->getWordStartTimeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t},\r\n";
    }

    out<<"\t\t],\r\n";

    out<<"\t\t\"phonemes\" : [\r\n";

    for(int i=0;i<sub->getPhonemeCount();i++)
    {
        out<<"\t\t\t{\r\n";
        out<<"\t\t\t\t\"word\" : \""<<sub->getPhonemeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"start\" : \""<<sub->getPhonemeStartTimeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"end\" : \""<<sub->getPhonemeEndTimeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t\t\"duration\" : \""<<sub->getPhonemeEndTimeByIndex(i) - sub->getPhonemeStartTimeByIndex(i)<<"\",\r\n";
        out<<"\t\t\t},\r\n";
    }

    out<<"\t\t]\r\n";
    out<<"\t},\r\n";

    out.close();
    return true;
}


bool printTranscriptionAsJSONContinuous(const std::string& fileName, AlignedData *alignedData, int printedTillIndex)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    for(int i=printedTillIndex;i<alignedData->_words.size();i++)
    {
        out<<"\t{\r\n";
        out<<"\t\t\"word\" : \""<<alignedData->_words[i]<<"\",\r\n";
        out<<"\t\t\"confidence\" : \""<<alignedData->_wordConf[i]<<"\",\r\n";
        out<<"\t\t\"start\" : \""<<alignedData->_wordStartTimes[i]<<"\",\r\n";
        out<<"\t\t\"end\" : \""<<alignedData->_wordEndTimes[i]<<"\",\r\n";
        out<<"\t\t\"duration\" : \""<<alignedData->_wordEndTimes[i] - alignedData->_wordStartTimes[i]<<"\",\r\n";
        out<<"\t},\r\n";
    }

    out.close();
    return true;
}

bool printKaraoke(const std::string& fileName, std::vector<SubtitleItem *> subtitles, outputOptions printOption)
{
    initFile(fileName, karaoke);

    int subCount = 1;

    for(SubtitleItem *sub : subtitles)
    {
        subCount = printKaraokeContinuous(fileName, subCount, sub, printOption);
    }

    return true;
}

int printKaraokeContinuous(const std::string& fileName, int subCount, SubtitleItem *sub, outputOptions printOption)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary | std::ofstream::app);

    for(int i=0;i<sub->getWordCount();i++)
    {
        int hh1,mm1,ss1,ms1;
        int hh2,mm2,ss2,ms2;
        char timeline[128];

        ms_to_srt_time(sub->getWordStartTimeByIndex(i),&hh1,&mm1,&ss1,&ms1);
        ms_to_srt_time(sub->getWordEndTimeByIndex(i),&hh2,&mm2,&ss2,&ms2);

        //printing in SRT format
        sprintf(timeline, "%02d:%02d:%02d,%03d --> %02d:%02d:%02d,%03d\n", hh1, mm1, ss1, ms1, hh2, mm2, ss2, ms2);

        out<<subCount++<<"\n";
        out<<timeline;

        std::string outputLine = "";

        for(int j=0;j<sub->getWordCount();j++)
        {
            if(j == i)
            {
                if(printOption == printAsKaraokeWithDistinctColors)
                {
                    if(sub->getWordRecognisedStatusByIndex(j))
                        outputLine += "<font color='#0000FF'>";

                    else
                        outputLine += "<font color='#ff0000'>";
                }

                else
                    outputLine += "<font color='#0000FF'>";

                outputLine += sub->getWordByIndex(j);
                outputLine += " </font>";
            }

            else
                outputLine += sub->getWordByIndex(j) + " ";
        }

        outputLine += "\n\n";
        out<<outputLine;
    }

    return subCount;
}



