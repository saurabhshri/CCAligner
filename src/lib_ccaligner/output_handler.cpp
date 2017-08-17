//
// Created by Saurabh Shrivastava on 17/08/17.
//

#include "output_handler.h"

inline bool initFile(std::string fileName, outputFormats outputFormat)
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

inline bool printFileEnd(std::string fileName, outputFormats outputFormat)
{
    std::ofstream out;
    out.open(fileName, std::ofstream::binary);

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

bool printSRT(std::string fileName, std::vector<SubtitleItem *> subtitles, outputOptions printOption)
{
    initFile(fileName, srt);

    int subCount = 1;

    for(SubtitleItem *sub : subtitles)
    {
        for(int i=0;i<sub->getWordCount();i++)
        {
            subCount = printSRTContinuos(fileName, subCount, sub, printOption);
        }
    }

    return true;
}


inline int printSRTContinuos(std::string fileName, int subCount, SubtitleItem *sub, outputOptions printOption)
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

bool printXML(std::string fileName, std::vector<SubtitleItem *> subtitles)
{
    initFile(fileName, xml);

    for(SubtitleItem *sub : subtitles)
    {
        printXMLContinuos(fileName, sub);
    }

    printFileEnd(fileName, xml);
    return true;
}

bool printXMLContinuos(std::string fileName, SubtitleItem *sub)
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
        out<<"\t\t\t\t</phoneme>\r\n";
    }
    out<<"\t\t\t</phonemes>\r\n";

    out<<"\t\t<end>"<<sub->getEndTime()<<"</end>\r\n";
    out<<"\t</subtitle>\r\n";

    out.close();
    return true;
}

bool printJSON(std::string fileName, std::vector<SubtitleItem *> subtitles)
{
    initFile(fileName, json);

    for(SubtitleItem *sub : subtitles)
    {
        printJSONContinuos(fileName, sub);
    }

    printFileEnd(fileName, json);

    return true;
}

bool printJSONContinuos(std::string fileName, SubtitleItem *sub)
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
        out<<"\t\t\t},\r\n";
    }

    out<<"\t\t],\r\n";
    out<<"\t},\r\n";

    out.close();
    return false;
}

bool printKaraoke(std::string fileName, std::vector<SubtitleItem *> subtitles, outputOptions printOption)
{
    initFile(fileName, karaoke);

    int subCount = 1;

    for(SubtitleItem *sub : subtitles)
    {
        subCount = printKaraokeContinuos(fileName, subCount, sub, printOption);
    }

    return true;
}

int printKaraokeContinuos(std::string fileName, int subCount, SubtitleItem *sub, outputOptions printOption)
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
