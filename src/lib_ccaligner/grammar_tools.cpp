/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "grammar_tools.h"

bool generate(std::vector <SubtitleItem*> subtitles, grammarName name)
{
    //create temporary directories in case it doesn't exist
    LOG("Creating temp directories at tempFiles/");

    int rv = std::system("mkdir -p tempFiles/corpus tempFiles/dict tempFiles/vocab tempFiles/fsg tempFiles/lm");

    if (WIFEXITED(rv) && WEXITSTATUS(rv) != 0)
    {
        FATAL(EXIT_FAILURE, "Unable to create directory tempFiles/ : %s", strerror(errno));
    }

    LOG("Directories created successfully!");

    std::ofstream corpusDump, fsgDump, vocabDump, dictDump;

    //setting exceptions to be thrown on failure
    corpusDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsgDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vocabDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    dictDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);


    if(name == corpus || name == complete_grammar)
    {
        std::cout<<"Creating Corpus : tempFiles/corpus/corpus.txt\n";

        try
        {
            corpusDump.open("tempFiles/corpus/corpus.txt",std::ios::binary);
        }

        catch(std::system_error& e)
        {
            FATAL(EXIT_FAILURE, e.code().message().c_str());
        }

        corpusDump.close();
    }

    for(SubtitleItem *sub : subtitles)
    {
        if(name == corpus || name == complete_grammar)
        {
            try
            {
                corpusDump.open("tempFiles/corpus/corpus.txt", std::ios::binary | std::ios::app);
            }

            catch(std::system_error& e)
            {
                FATAL(EXIT_FAILURE, e.code().message().c_str());
            }

            corpusDump << "<s> " << StringToLower(sub->getDialogue()) << " </s>\n";
            corpusDump.close();
        }

        if(name == fsg || name == complete_grammar)
        {
            long int startTime = sub->getStartTime();
            std::string fsgFileName("tempFiles/fsg/" + std::to_string(startTime));
            fsgFileName += ".fsg";

            try
            {
                fsgDump.open(fsgFileName, std::ios::binary);
            }

            catch(std::system_error& e)
            {
                FATAL(EXIT_FAILURE, e.code().message().c_str());
            }

            int numberOfWords = sub->getWordCount();

            fsgDump<<"FSG_BEGIN CUSTOM_FSG\n";
            fsgDump<<"NUM_STATES "<<(numberOfWords * 2) + 2<<"\n";
            fsgDump<<"START_STATE 0\n";
            fsgDump<<"FINAL_STATE "<<(numberOfWords * 2) + 1<<"\n\n";
            fsgDump<<"# Transitions\n";

            for(int i=0;i<numberOfWords;i++)
            {
                fsgDump<<"TRANSITION "<<"0 "<<i+1<<" 0.0909\n";
            }
            for(int i=numberOfWords, counter = 1;counter<=numberOfWords;i++,counter++)
            {
                fsgDump<<"TRANSITION "<<counter<<" "<<i+1<<" 1.0 "<<StringToLower(sub->getWordByIndex(counter-1))<<"\n";
            }
            for(int i=numberOfWords + 1;i<numberOfWords * 2 + 1;i++)
            {
                fsgDump<<"TRANSITION "<<i<<" "<<numberOfWords * 2 + 1<<" 0.0909\n";
            }

            fsgDump<<"TRANSITION "<<numberOfWords * 2 + 1<<" 0 0.0909\n";


            fsgDump<<"FSG_END\n";
            fsgDump.close();
        }

    }

    if(name == vocab || name == complete_grammar)
    {
        rv = std::system("text2wfreq < tempFiles/corpus/corpus.txt | wfreq2vocab > tempFiles/vocab/complete.vocab");

        if (WIFEXITED(rv) && WEXITSTATUS(rv) != 0)
        {
            FATAL(EXIT_FAILURE, "Something went wrong while creating vocabulary!");
        }
    }

    if(name == dict || name == complete_grammar)
    {
        std::cout<<"Creating Dictionary, this might take a little time depending"
            "on your TensorFlow configuration : tempFiles/dict/complete.dict\n";
        rv = std::system("g2p-seq2seq --decode tempFiles/vocab/complete.vocab --model g2p-seq2seq-cmudict/ > tempFiles/dict/complete.dict");

        if (WIFEXITED(rv) && WEXITSTATUS(rv) != 0)
        {
            FATAL(EXIT_FAILURE, "Something went wrong while creating dictionary!");
        }
    }

    if(name == lm || name == complete_grammar )
    {
        std::cout<<"Creating Biased Language Model : tempFiles/lm/complete.lm\n";

        rv = std::system("text2idngram -vocab tempFiles/vocab/complete.vocab -idngram tempFiles/lm/lm.idngram <  tempFiles/corpus/corpus.txt");

        if (WIFEXITED(rv) && WEXITSTATUS(rv) != 0)
        {
            FATAL(EXIT_FAILURE, "Something went wrong while creating idngram file!");
        }

        rv = std::system("idngram2lm -vocab_type 0 -idngram tempFiles/lm/lm.idngram -vocab tempFiles/vocab/complete.vocab  -arpa tempFiles/lm/complete.lm");

        if (WIFEXITED(rv) && WEXITSTATUS(rv) != 0)
        {
            FATAL(EXIT_FAILURE, "Something went wrong while creating biased language model!");
        }
    }

    LOG("Grammar files created!");

    return true;

}
