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
    outputFileName = "";
    modelPath = "model/";
    lmPath = "tempFiles/lm/complete.lm";
    dictPath = "tempFiles/dict/complete.dict";
    fsgPath = "tempFiles/fsg/";

    phoneticlmPath = "model/en-us-phone.lm.bin";

    //using date and time for log file name
    std::time_t t = std::time(NULL);
    std::tm * lt = std::localtime(&t);
    char buffer [18];
    std::strftime(buffer, 15, "%d-%m-%Y_%H%M", lt);
    std::string localTime(buffer);
    logPath = "tempFiles/" + localTime + ".log";
    phonemeLogPath = "tempFiles/phoneme-" + localTime + ".log";
    delete(lt);

    sampleWindow = 0;
    audioWindow = 0;
    searchWindow = 3;


    chosenAlignerType = asrAligner;
    grammarType = complete_grammar;
    outputFormat = xml;
    printOption = printBothWithDistinctColors;

    verbosity = true;
    useFSG = false;
    transcribe = false;
    useBatchMode = false;
    useExperimentalParams = false;
    searchPhonemes = false;
    displayRecognised = true;
    readStream = false;
    quickDict = false;
    quickLM = false;
}

void Params::inputParams(int argc, char *argv[])
{
    for(int i=1;i<argc;i++)         //parsing arguments
    {
        std::string subParam, paramPrefix(argv[i]);

        if(i+1 < argc)
            subParam = argv[i+1];

        else
            subParam = "";

        if(paramPrefix== "-wav")
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

        else if(paramPrefix== "-" || paramPrefix== "-stdin")
        {
            readStream = true;
        }

        else if(paramPrefix== "-out")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            outputFileName = subParam;
            i++;
        }

        else if(paramPrefix== "-oFormat")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "srt")
                outputFormat = srt;

            else if(subParam  == "xml")
                outputFormat = xml;

            else if(subParam  == "json")
                outputFormat = json;

            else if(subParam  == "karaoke")
                outputFormat = karaoke;

            else if(subParam  == "stdout")
                outputFormat = console;

            else
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

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

        else if(paramPrefix == "-phoneLM")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            phoneticlmPath = subParam;
            i++;
        }

        else if(paramPrefix == "-phoneLog")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            phonemeLogPath = subParam;
            i++;
        }

        else if(paramPrefix == "--enable-phonemes")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam == "yes")
                searchPhonemes = true;

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

        else if(paramPrefix == "--quick-dict")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam == "yes")
                quickDict = true;

            i++;
        }

        else if(paramPrefix == "--quick-lm")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam == "yes")
                quickLM = true;

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
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "yes")
                transcribe = true;

            i++;
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

        else if(paramPrefix== "--display-recognised")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            if(subParam  == "no")
                displayRecognised = false;

            i++;
        }

        else if(paramPrefix== "-searchWindow")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            searchWindow = std::strtoul( subParam.c_str(), NULL, 10 );

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

            sampleWindow = std::strtoul( subParam.c_str(), NULL, 10 );

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

            audioWindow = std::strtoul( subParam.c_str(), NULL, 10 );

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

        else
        {
            FATAL(EXIT_INVALID_PARAMETERS, "Parameter '%s' not recognised!", paramPrefix.c_str());
        }

    }

    validateParams();

}

void Params::validateParams()
{
    if(audioFileName.empty() && !readStream)
        FATAL(EXIT_INVALID_PARAMETERS, "Audio file name is empty!");

    if(subtitleFileName.empty())
        FATAL(EXIT_INVALID_PARAMETERS, "Subtitle file name is empty!");

    if(modelPath.empty())
        LOG("Using default Model Path.");

    if(dictPath.empty())
        LOG("Using default Dictionary Path.");

    if(lmPath.empty())
        LOG("Using default LM Path.");

    if(logPath.empty())
        LOG("Using default Log Path.");

    if(fsgPath.empty())
        LOG("Using default FSG Path.");

    if(phoneticlmPath.empty())
        LOG("Using default Phonetic LM Path.");

    if(phonemeLogPath.empty())
        LOG("Using default Phoneme Log Path.");

    if(readStream)
    {
        audioFileName = "stdin";
    }

    if(outputFileName.empty())
    {
        outputFileName = extractFileName(audioFileName);

        switch (outputFormat)  //decide on based of set output format
        {
            case srt:       outputFileName += ".srt";
                break;

            case xml:       outputFileName += ".xml";
                break;

            case json:      outputFileName += ".json";
                break;

            case karaoke:   outputFileName += ".srt";
                break;

            default:        FATAL(EXIT_UNKNOWN, "An error occurred while choosing output format!");
                exit(2);
        }
    }

    if(grammarType == complete_grammar && quickDict)
        grammarType = quick_dict;

    if(grammarType == complete_grammar && quickLM)
        grammarType = quick_lm;

    if(useFSG && transcribe)
    {
        FATAL(EXIT_INCOMPATIBLE_PARAMETERS , "FSG and Transcribing are not compatible!");
    }

    if(searchPhonemes && transcribe)
    {
        FATAL(EXIT_INCOMPATIBLE_PARAMETERS , "Sorry, currently phoneme transcribing is not supported!");
    }

    if(verbosity)
    {
        should_log = true;
        printParams();
    }
}

void Params::printParams()
{
    std::cout<<"audioFileName       : "<<audioFileName<<"\n";
    std::cout<<"subtitleFileName    : "<<subtitleFileName<<"\n";
    std::cout<<"outputFileName      : "<<outputFileName<<"\n";
    std::cout<<"modelPath           : "<<modelPath<<"\n";
    std::cout<<"lmPath              : "<<lmPath<<"\n";
    std::cout<<"dictPath            : "<<dictPath<<"\n";
    std::cout<<"fsgPath             : "<<fsgPath<<"\n";

    std::cout<<"phoneticlmPath      : "<<phoneticlmPath<<"\n";

    std::cout<<"logPath             : "<<logPath<<"\n";
    std::cout<<"phonemeLogPath      : "<<phonemeLogPath<<"\n";

    std::cout<<"sampleWindow        : "<<sampleWindow<<"\n";
    std::cout<<"audioWindow         : "<<audioWindow<<"\n";
    std::cout<<"searchWindow        : "<<searchWindow<<"\n";


    std::cout<<"chosenAlignerType   : "<<chosenAlignerType<<"\n";
    std::cout<<"grammarType         : "<<grammarType<<"\n";
    std::cout<<"outputFormat        : "<<outputFormat<<"\n";
    std::cout<<"printOption         : "<<printOption<<"\n";

    std::cout<<"verbosity           : "<<verbosity<<"\n";
    std::cout<<"useFSG              : "<<useFSG<<"\n";
    std::cout<<"transcribe          : "<<transcribe<<"\n";
    std::cout<<"useBatchMode        : "<<useBatchMode<<"\n";
    std::cout<<"ExperimentalParams  : "<< useExperimentalParams<<"\n";
    std::cout<<"searchPhonemes      : "<<searchPhonemes<<"\n\n\n";

    std::cout<<"=====================================================\n\n";

}

Params::~Params()
{

}
