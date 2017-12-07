/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "grammar_tools.h"

static int systemGetStatus(const char* command) {
    int rv = std::system(command);

#ifndef WIN32
    if (WIFEXITED(rv) == 0) return 0;
    return WEXITSTATUS(rv);
#else
    return rv;
#endif
}

void ConfigureQuickGenerationOptions(bool &generateQuickDict, bool &generateQuickLM, grammarName &name) //Setup quick grammar generation
{
    if (name == quick_dict)
    {
        generateQuickDict = true;
        name = complete_grammar;
    }

    else if (name == quick_lm)
    {
        generateQuickLM = true;
        name = complete_grammar;
    }
}

void CreateTempDirectories() // Create temporary directories
{
    //create temporary directories in case they don't exist
    DEBUG << "Creating temporary directories at tempFiles/";

#ifndef WIN32
    int rv = systemGetStatus("mkdir -p tempFiles/corpus tempFiles/dict tempFiles/vocab tempFiles/fsg tempFiles/lm");
#else
    int rv = systemGetStatus("if not exist tempFiles\\ mkdir tempFiles\\corpus tempFiles\\dict tempFiles\\vocab tempFiles\\fsg tempFiles\\lm");
#endif

    if (rv != 0)
    {
        FATAL(UnknownError) << "Unable to create directory tempFiles/ : " << strerror(errno);
    }

    DEBUG << "Directories created successfully!";
}

void CreateNewGrammarFiles(grammarName name, std::ofstream &corpusDump, std::ofstream &fsgDump,
    std::ofstream &vocabDump, std::ofstream &dictDump, std::ofstream &phoneticCorpusDump, std::ofstream &logDump) //Create and initialize files for writing.
{
    //setting exceptions to be thrown on failure
    corpusDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fsgDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    vocabDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    dictDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    phoneticCorpusDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    logDump.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        logDump.open("tempFiles/grammar.log", std::ios::binary);
        logDump.close();
    }
    catch (std::system_error& e)
    {
        FATAL(UnknownError) << e.code().message();
    }

    if (name == corpus || name == complete_grammar)
    {
        INFO << "Creating Corpus : tempFiles/corpus/corpus.txt";

        try
        {
            corpusDump.open("tempFiles/corpus/corpus.txt", std::ios::binary);
            corpusDump.close();
        }
        catch (std::system_error& e)
        {
            FATAL(UnknownError) << e.code().message();
        }
    }

    if (name == phone_lm || name == complete_grammar)
    {
        INFO << "Creating Phonetic Corpus : tempFiles/corpus/phoneticCorpus.txt";

        try
        {
            phoneticCorpusDump.open("tempFiles/corpus/phoneticCorpus.txt", std::ios::binary);
            phoneticCorpusDump.close();
        }
        catch (std::system_error& e)
        {
            FATAL(UnknownError) << e.code().message();
        }
    }
}

void CreateBiasedLM(grammarName name, bool generateQuickLM) //Create biased language model
{
    int rv;
    if (name == lm || name == complete_grammar)
    {
        INFO << "Creating Biased Language Model : tempFiles/lm/complete.lm";

        if (generateQuickLM)
        {
            rv = systemGetStatus("perl quick_lm.pl -s tempFiles/corpus/corpus.txt 2>tempFiles/grammar.log");

            if (rv != 0)
                FATAL(UnknownError) << "Something went wrong while creating Phonetic Language Model!";

#ifndef WIN32
            rv = systemGetStatus("mv tempFiles/corpus/corpus.txt.arpabo tempFiles/lm/complete.lm 2>tempFiles/grammar.log");
#else
            rv = systemGetStatus("move tempFiles\\corpus\\corpus.txt.arpabo tempFiles\\lm\\complete.lm 2>tempFiles\\grammar.log");
#endif
            if (rv != 0)
                FATAL(UnknownError) << "Something went wrong while moving phonetic model!";
        }
        else
        {
            rv = systemGetStatus("text2idngram -vocab tempFiles/vocab/complete.vocab -idngram tempFiles/lm/lm.idngram <  tempFiles/corpus/corpus.txt 2>tempFiles/grammar.log");

            if (rv != 0)
                FATAL(UnknownError) << "Something went wrong while creating idngram file!";

            rv = systemGetStatus("idngram2lm -vocab_type 0 -idngram tempFiles/lm/lm.idngram -vocab tempFiles/vocab/complete.vocab  -arpa tempFiles/lm/complete.lm 2>tempFiles/grammar.log");

            if (rv != 0)
                FATAL(UnknownError) << "Something went wrong while creating biased language model!";
        }
    }
}

void GenerateDict(bool generateQuickDict) // Generate dictionary from tensor flow (or not if making quick dict)
{
    if (generateQuickDict)
    {
        std::ifstream vocabInput("tempFiles/vocab/complete.vocab");
        std::ofstream dictDump;

        try
        {
            dictDump.open("tempFiles/dict/complete.dict", std::ios::binary);
        }
        catch (std::system_error& e)
        {
            FATAL(UnknownError) << e.code().message();
        }

        std::string word;
        std::vector<std::string> phonemes;
        bool beginWriting = false;

        while (std::getline(vocabInput, word))
        {
            if (beginWriting)
            {
                phonemes = stringToPhoneme(word);

                dictDump << word << " ";

                for (std::string ph : phonemes)
                    dictDump << ph << " ";
                dictDump << "\n";
            }

            if (word == "<s>")
                beginWriting = true;
        }

        vocabInput.close();
        dictDump.close();
    }
    else
    {
        INFO << "Creating the Dictionary, this might take a little time depending "
            "on your TensorFlow configuration : tempFiles/dict/complete.dict";
        int rv = systemGetStatus("g2p-seq2seq --decode tempFiles/vocab/complete.vocab --model g2p-seq2seq-cmudict/ > tempFiles/dict/complete.dict");

        if (rv != 0)
        {
            FATAL(UnknownError) << "Something went wrong while creating dictionary!";
        }
    }

}


std::string getFileData(std::string _fileName)           //returns whole read file. Used to read text files
{
    std::ifstream infile(_fileName);
    std::string allData = "";
    std::string line;
    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        allData += line + "\n";
    }
    return allData;
}

bool generate(std::string transcriptFileName, grammarName name) //Generate Grammar from text files.
{
    std::string transcript = getFileData(transcriptFileName);

    bool generateQuickDict = false, generateQuickLM = false;
    int rv;

    ConfigureQuickGenerationOptions(generateQuickDict, generateQuickLM, name);

    CreateTempDirectories();

    std::ofstream corpusDump, phoneticCorpusDump, fsgDump, vocabDump, dictDump, logDump;

    CreateNewGrammarFiles(name, corpusDump, fsgDump, vocabDump, dictDump, phoneticCorpusDump, logDump);

    //Writing Files
    if (name == corpus || name == complete_grammar)
    {
        try
        {
            corpusDump.open("tempFiles/corpus/corpus.txt", std::ios::binary | std::ios::app);
        }
        catch (std::system_error& e)
        {
            FATAL(UnknownError) << e.code().message();
        }

        corpusDump << "<s> " << stringToLower(transcript) << " </s>\n";
        corpusDump.close();
    }

	if (name == phone_lm || name == complete_grammar)
	{
		try
		{
			phoneticCorpusDump.open("tempFiles/corpus/phoneticCorpus.txt", std::ios::binary | std::ios::app);
		}

		catch (std::system_error& e)
		{
			FATAL(UnknownError) << e.code().message();
		}

		std::istringstream iss(transcript);
		std::vector<std::string> SplitTranscript((std::istream_iterator<std::string>(iss)),
			std::istream_iterator<std::string>()); //split transcript into words

		int numberOfWords = SplitTranscript.size();
		std::string printPhoneticCourpus = "SIL ";

		for (int i = 0; i<numberOfWords; i++)
		{
			std::vector<Phoneme> phones = stringToPhoneme(stringToLower(SplitTranscript[i]));

			for (Phoneme ph : phones)
				printPhoneticCourpus += ph + " ";
		}

		printPhoneticCourpus += "SIL\n";

		phoneticCorpusDump << printPhoneticCourpus;
		phoneticCorpusDump.close();
	}

    //FSG not needed for transcription

    if (name == vocab || name == complete_grammar)
    {
        rv = systemGetStatus("text2wfreq < tempFiles/corpus/corpus.txt 2>tempFiles/grammar.log | wfreq2vocab > tempFiles/vocab/complete.vocab 2>tempFiles/grammar_wfreq2vocab.log");

        if (rv != 0)
            FATAL(UnknownError) << "Something went wrong while creating vocabulary!";
    }

    if (name == dict || name == complete_grammar)
    {
        GenerateDict(generateQuickDict);
    }

    CreateBiasedLM(name, generateQuickLM);

    DEBUG<<"Grammar files created!";
    return true;
}

bool generate(std::vector <SubtitleItem*> subtitles, grammarName name) //Generate grammar from subtitle (.srt) files.
{
    bool generateQuickDict = false, generateQuickLM = false;
    int rv;

    ConfigureQuickGenerationOptions(generateQuickDict, generateQuickLM, name);

    CreateTempDirectories();

    std::ofstream corpusDump, phoneticCorpusDump, fsgDump, vocabDump, dictDump, logDump;

    CreateNewGrammarFiles(name, corpusDump, fsgDump, vocabDump, dictDump, phoneticCorpusDump, logDump);

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
                FATAL(UnknownError) << e.code().message();
            }

            corpusDump << "<s> " << stringToLower(sub->getDialogue()) << " </s>\n";
            corpusDump.close();
        }

        if(name == phone_lm || name == complete_grammar)
        {
            try
            {
                phoneticCorpusDump.open("tempFiles/corpus/phoneticCorpus.txt", std::ios::binary | std::ios::app);
            }

            catch(std::system_error& e)
            {
                FATAL(UnknownError) << e.code().message();
            }

            int numberOfWords = sub->getWordCount();
            std::string printPhoneticCourpus = "SIL ";

            for(int i=0;i<numberOfWords;i++)
            {
                std::vector<Phoneme> phones = stringToPhoneme(stringToLower(sub->getWordByIndex(i)));

                for(Phoneme ph : phones)
                    printPhoneticCourpus += ph + " ";
            }

            printPhoneticCourpus += "SIL\n";

            phoneticCorpusDump << printPhoneticCourpus;
            phoneticCorpusDump.close();
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
                FATAL(UnknownError) << e.code().message();
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
                fsgDump<<"TRANSITION "<<counter<<" "<<i+1<<" 1.0 "<<stringToLower(sub->getWordByIndex(counter-1))<<"\n";
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
        DEBUG << "Creating vocabulary...";

        rv = systemGetStatus("text2wfreq < tempFiles/corpus/corpus.txt 2>tempFiles/grammar.log | wfreq2vocab > tempFiles/vocab/complete.vocab 2>tempFiles/grammar_wfreq2vocab.log");

        if (rv != 0)
        {
            FATAL(UnknownError) << "Something went wrong while creating vocabulary!";
        }

        DEBUG << "Vocabulary created!";
    }

    if(name == dict || name == complete_grammar)
    {
        GenerateDict(generateQuickDict);
    }

    CreateBiasedLM(name, generateQuickLM);

    if (name == phone_lm || name == complete_grammar)
    {
        INFO << "Creating Phonetic Language Model : tempFiles/lm/phone.lm";

        rv = systemGetStatus("perl quick_lm.pl -s tempFiles/corpus/phoneticCorpus.txt 2>tempFiles/grammar.log");
        if (rv != 0)
            FATAL(UnknownError) << "Something went wrong while creating Phonetic Language Model!";

#ifndef WIN32
        rv = systemGetStatus("mv tempFiles/corpus/phoneticCorpus.txt.arpabo tempFiles/lm/ 2>tempFiles/grammar.log");
#else
        rv = systemGetStatus("move tempFiles\\corpus\\phoneticCorpus.txt.arpabo tempFiles\\lm\\ 2>tempFiles\\grammar.log");
#endif
        if (rv != 0)
            FATAL(UnknownError) << "Something went wrong while moving phonetic model!";
    }

    DEBUG << "Grammar files created!";

    return true;
}
