/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 */

#include "params.h"


Params::Params()
{
    audioFileName = "";
    subtitleFileName = "";
    modelPath = "model/";
    lmPath = "tempFiles/lm/complete.lm";
    dictPath = "tempFiles/dict/complete.dict";
    fsgPath = "tempFiles/fsg/";

    //using date and time for log file name
    std::time_t t = std::time(NULL);
    std::tm * lt = std::localtime(&t);
    char buffer [18];
    std::strftime(buffer, 15, "%d-%m-%Y_%H%M", lt);
    std::string localTime(buffer);
    logPath = "tempFiles/" + localTime + ".log";

    searchWindow = 0;
    sampleWindow = 0;
    audioWindow = 0;

    chosenAlignerType = asrAligner;

    grammarType = complete_grammar;

    printOption = printBothWithDistinctColors;

    verbosity = true;
    useFSG = false;
    transcribe = false;
    useBatchMode = false;
    useExperimentalParams = false;
}

void Params::inputParams(int argc, char *argv[])
{
    for(int i=1;i<argc;i++)         //parsing arguments
    {
        std::string paramPrefix(argv[i]), subParam(argv[i+1]);

        if(paramPrefix== "-in")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            audioFileName = subParam;
            i++;
        }

        else if(paramPrefix== "-srt")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            subtitleFileName = subParam;
            i++;
        }

        else if(paramPrefix == "-model")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            modelPath = subParam;
            i++;
        }

        else if(paramPrefix== "-lm")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            lmPath = subParam;
            i++;
        }


        else if(paramPrefix== "-dict")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            dictPath = subParam;
            i++;
        }


        else if(paramPrefix== "-fsg")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            fsgPath = subParam;
            i++;
        }


        else if(paramPrefix== "-log")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            logPath = subParam;
            i++;
        }

        else if(paramPrefix== "--generate-grammar")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                grammarType = complete_grammar;

            else if(subParam  == "no")
                grammarType = no_grammar;

            else if(subParam  == "onlyCorpus")
                grammarType = corpus;

            else if(subParam  == "onlyDict")
                grammarType = dict;

            else if(subParam  == "onlyFSG")
                grammarType = fsg;

            else if(subParam  == "onlyLM")
                grammarType = lm;

            else if(subParam  == "onlyVocab")
                grammarType = vocab;

            else
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            i++;
        }

        else if(paramPrefix== "--print-aligned")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                printOption = printOnlyRecognised;

            else if(subParam  == "no")
                printOption = printBothWihoutColors ;

            else if(subParam  == "withColors")
                printOption = printBothWithDistinctColors;

            else if(subParam  == "karaoke")
                printOption = printAsKaraoke;

            else if(subParam  == "karaokeWithColors")
                printOption = printAsKaraoke;

            else
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            i++;
        }

        else if(paramPrefix== "--use-fsg")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                useFSG = true;

            i++;
        }

        else if(paramPrefix== "-transcribe")
        {
            transcribe = true;
        }


        else if(paramPrefix== "-verbose")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "no")
                verbosity = false;

            i++;
        }

        else if(paramPrefix== "-searchWindow")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            searchWindow = strtoull( subParam.c_str(), NULL, 10 );

            if ( errno )
            {
                fprintf( stderr, "%s: parameter %s: %s\n", argv[0], argv[i], strerror(errno) );
            }

            i++;
        }

        else if(paramPrefix== "-sampleWindow")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            sampleWindow = strtoull( subParam.c_str(), NULL, 10 );

            if ( errno )
            {
                fprintf( stderr, "%s: parameter %s: %s\n", argv[0], argv[i], strerror(errno) );
            }

            i++;
        }

        else if(paramPrefix== "-audioWindow")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            audioWindow = strtoull( subParam.c_str(), NULL, 10 );

            if ( errno )
            {
                fprintf( stderr, "%s: parameter %s: %s\n", argv[0], argv[i], strerror(errno) );
            }

            i++;
        }

        else if(paramPrefix== "-useBatchMode")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                useBatchMode = true;

            i++;
        }

        else if(paramPrefix== "-experiment")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                useExperimentalParams = true;

            i++;
        }

        else if(paramPrefix== "-approx")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                chosenAlignerType = approxAligner;

            i++;
        }

    }

}
