/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "grammar_tools.h"


std::string StringToLower(std::string strToConvert)
{
    std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::tolower);

    return strToConvert;
}

bool generate(std::vector <SubtitleItem*> subtitles, grammarName name)
{
    std::system("mkdir -p tempFiles/corpus tempFiles/dict tempFiles/vocab tempFiles/fsg tempFiles/lm");

    std::ofstream corpusDump, fsgDump, vocabDump, dictDump;


    if(name == corpus || name == all)
    {
        corpusDump.open("tempFiles/corpus/corpus.txt",std::ios::binary);
    }

    for(SubtitleItem *sub : subtitles)
    {
        if(name == corpus || name == all)
        {
            corpusDump.open("tempFiles/corpus/corpus.txt", std::ios::binary | std::ios::app);
            corpusDump << "<s> " << StringToLower(sub->getDialogue()) << " </s>\n";
            corpusDump.close();
        }

        if(name == fsg || name == all)
        {
            long int startTime = sub->getStartTime();
            std::string fsgFileName("tempFiles/fsg/" + std::to_string(startTime));
            fsgFileName += ".fsg";

            fsgDump.open(fsgFileName, std::ios::binary);
            int numberOfWords = sub->getWordCount();

            fsgDump<<"FSG_BEGIN CUSTOM_FSG\n";
            fsgDump<<"NUM_STATES "<<numberOfWords + 1<<"\n";
            fsgDump<<"START_STATE 0\n";
            fsgDump<<"FINAL_STATE "<<numberOfWords<<"\n\n";
            fsgDump<<"# Transitions\n";

            for(int i=0;i<numberOfWords;i++)
            {
                fsgDump<<"TRANSITION "<<i<<" "<<i+1<<" 1.0 "<<StringToLower(sub->getWordByIndex(i))<<"\n";
            }

            fsgDump<<"FSG_END\n";
            fsgDump.close();
        }

    }

    if(name == vocab || name == all)
    {
        auto rv = std::system("text2wfreq < tempFiles/corpus/corpus.txt | wfreq2vocab > tempFiles/vocab/complete.vocab");
    }

    if(name == dict || name == all)
    {
        auto rv = std::system("g2p-seq2seq --decode tempFiles/vocab/complete.vocab --model /Users/saurabhshri/work/g2p-seq2seq-cmudict > tempFiles/dict/complete.dict");

    }

    if(name == lm || name == all )
    {
        auto rv = std::system("text2idngram -vocab tempFiles/vocab/complete.vocab -idngram tempFiles/lm/lm.idngram <  tempFiles/corpus/corpus.txt");
        rv = std::system("idngram2lm -vocab_type 0 -idngram tempFiles/lm/lm.idngram -vocab tempFiles/vocab/complete.vocab  -arpa tempFiles/lm/complete.lm");
    }

}
