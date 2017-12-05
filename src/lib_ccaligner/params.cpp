/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 */

#include "params.h"

// Default paths.
namespace
{
    constexpr auto defaultModelPath = "model/";
    constexpr auto defaultLmPath = "tempFiles/lm/complete.lm";
    constexpr auto defaultDictPath = "tempFiles/dict/complete.dict";
    constexpr auto defaultFsgPath = "tempFiles/fsg/";
    constexpr auto defaultPhoneticLmPath = "model/en-us-phone.lm.bin";
}

Params::Params() noexcept
    : localTime(32, '\0'),

      modelPath(defaultModelPath),
      lmPath(defaultLmPath),
      dictPath(defaultDictPath),
      fsgPath(defaultFsgPath),
      phoneticlmPath(defaultPhoneticLmPath),

      searchWindow(3),
      audioWindow(0),
      sampleWindow(0),

      chosenAlignerType(asrAligner),
      grammarType(complete_grammar),
      outputFormat(xml),
      printOption(printBothWithDistinctColors),

      verbosity(true),
      useFSG(),
      transcribe(),
      useBatchMode(),
      useExperimentalParams(),
      searchPhonemes(),
      displayRecognised(true),
      readStream(),
      quickDict(),
      quickLM(),
      audioIsRaw()
{
    // Using date and time for log filename.
    const auto now = std::time(nullptr);
    localTime.erase(std::strftime(&localTime.front(), localTime.size(), "%d-%m-%Y-%H-%M-%S", std::localtime(&now)));

    logPath = "tempFiles/" + localTime + ".log";
    phonemeLogPath = "tempFiles/phoneme-" + localTime + ".log";
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
                Fatal<incomplete_parameters_exception>() << "-wav requires a path to valid wave!";
            }

            audioFileName = subParam;
            i++;
        }
        else if (paramPrefix == "-raw") {
            if (i + 1 > argc) {
                Fatal<incomplete_parameters_exception>() << "-raw requires a path to valid raw wave!";
            }

            audioFileName = subParam;
            audioIsRaw = true;
            i++;
        }
        else if (paramPrefix == "--raw-stream") {
            audioIsRaw = true;
            readStream = true;
        }
        else if(paramPrefix== "-srt")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-srt requires a path to valid SubRip subtitle file!";
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
                Fatal<incomplete_parameters_exception>() << "-out requires a valid output filename!";
            }

            outputFileName = subParam;
            i++;
        }

        else if(paramPrefix== "-oFormat")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-oFormat requires a valid output format";
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
                Fatal<std::invalid_argument>() << "-oFormat requires a valid output format!";
            }

            i++;
        }

        else if(paramPrefix == "-model")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-model requires a path to valid acoustic model!";
            }

            modelPath = subParam;
            i++;
        }

        else if(paramPrefix== "-lm")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-lm requires a path to language model!";
            }

            lmPath = subParam;
            i++;
        }


        else if(paramPrefix== "-dict")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-dict requires a path to a valid dictionary file!";
            }

            dictPath = subParam;
            i++;
        }


        else if(paramPrefix== "-fsg")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-fsg requires a path to a valid directory containing FSG files!";
            }

            fsgPath = subParam;
            i++;
        }


        else if(paramPrefix== "-log")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-log requires a path to a valid output log file!";
            }

            logPath = subParam;
            i++;
        }

        else if(paramPrefix == "-phoneLM")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-phoneLM requires a path to a valid phonetic language model!";

            }

            phoneticlmPath = subParam;
            i++;
        }

        else if(paramPrefix == "-phoneLog")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-phoneLog requires a path to a valid output log file!";

            }

            phonemeLogPath = subParam;
            i++;
        }

        else if(paramPrefix == "--enable-phonemes")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--enable-phonemes requires a valid response!";

            }

            if(subParam == "yes")
                searchPhonemes = true;

            i++;
        }

        else if(paramPrefix== "--generate-grammar")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--generate-grammar requires a valid response!";

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
                Fatal<std::invalid_argument>() << "--generate-grammar requires a valid response!";
            }

            i++;
        }

        else if(paramPrefix == "--quick-dict")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--quick-dict requires a valid response!";
            }

            if(subParam == "yes")
                quickDict = true;

            i++;
        }

        else if(paramPrefix == "--quick-lm")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--quick-lm requires a valid response!";
            }

            if(subParam == "yes")
                quickLM = true;

            i++;
        }

        else if(paramPrefix== "--print-aligned")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--print-aligned requires a valid response!";
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
                Fatal<std::invalid_argument>() << "--print-aligned requires a valid response!";
            }

            i++;
        }

        else if(paramPrefix== "--use-fsg")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--use-fsg requires a valid response!";
            }

            if(subParam  == "yes")
                useFSG = true;

            i++;
        }

        else if(paramPrefix== "-transcribe")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-transcribe requires a valid response!";
            }

            if(subParam  == "yes")
                transcribe = true;

            i++;
        }


        else if(paramPrefix== "-verbose")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-verbose requires a valid response!";
            }

            if(subParam  == "no")
                verbosity = false;

            i++;
        }

        else if(paramPrefix== "--display-recognised")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "--display-recognised requires a valid response!";
            }

            if(subParam  == "no")
                displayRecognised = false;

            i++;
        }

        else if(paramPrefix== "-searchWindow")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-searchWindow requires an integer value to determine the search scope!";
            }

            searchWindow = std::strtoul( subParam.c_str(), nullptr, 10 );

            if ( errno )
            {
                Fatal<std::invalid_argument>() << "Invalid value passed to -searchWindow : " << strerror(errno);
            }

            i++;
        }

        else if(paramPrefix== "-sampleWindow")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-sampleWindow requires a valid integer value to determine the recognition scope!";
            }

            sampleWindow = std::strtoul( subParam.c_str(), nullptr, 10 );

            if ( errno )
            {
                Fatal<std::invalid_argument>() << "Invalid value passed to -sampleWindow : " << strerror(errno);
            }

            i++;
        }

        else if(paramPrefix== "-audioWindow")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-audioWindow requires a valid integer value in milliseconds to determine the recognition scope!";
            }

            audioWindow = std::strtoul( subParam.c_str(), nullptr, 10 );

            if ( errno )
            {
                Fatal<std::invalid_argument>() << "Invalid value passed to -audioWindow : " << strerror(errno);
            }

            i++;
        }

        else if(paramPrefix== "-useBatchMode")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-useBatchMode requires a valid response!";
            }

            if(subParam  == "yes")
                useBatchMode = true;

            i++;
        }

        else if(paramPrefix== "-experiment")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-experiment requires a valid response!";
            }

            if(subParam  == "yes")
                useExperimentalParams = true;

            i++;
        }

        else if(paramPrefix== "-approx")
        {
            if(i+1 > argc)
            {
                Fatal<incomplete_parameters_exception>() << "-approx requires a valid response!";
            }

            if(subParam  == "yes")
                chosenAlignerType = approxAligner;

            i++;
        }

        else
        {
            Fatal<std::invalid_argument>() << "Parameter " << paramPrefix << " is not recognised!";
        }

    }

    validateParams();

}

void Params::validateParams()
{
    if(audioFileName.empty() && !readStream)
        Fatal<std::invalid_argument>() << "Audio file name is empty!";

    if(subtitleFileName.empty())
        Fatal<std::invalid_argument>() << "Subtitle file name is empty!";

    if(modelPath.empty())
        Log() << "Using default Model Path.";

    if(dictPath.empty())
        Log() << "Using default Dictionary Path.";

    if(lmPath.empty())
        Log() << "Using default LM Path.";

    if(logPath.empty())
        Log() << "Using default Log Path.";

    if(fsgPath.empty())
        Log() << "Using default FSG Path.";

    if(phoneticlmPath.empty())
        Log() << "Using default Phonetic LM Path.";

    if(phonemeLogPath.empty())
        Log() << "Using default Phoneme Log Path.";

    if(readStream)
    {
        audioFileName = "stdin";
    }

    if(outputFileName.empty())
    {
        outputFileName = extractFileName(audioFileName);

        switch (outputFormat)  //decide on basis of set output format
        {
            case srt:       outputFileName += ".srt";
                break;

            case xml:       outputFileName += ".xml";
                break;

            case json:      outputFileName += ".json";
                break;

            case karaoke:   outputFileName += ".srt";
                break;

            default:    Fatal<std::invalid_argument>() << "Invalid format for output!";
        }
    }

    if(grammarType == complete_grammar && quickDict)
        grammarType = quick_dict;

    if(grammarType == complete_grammar && quickLM)
        grammarType = quick_lm;

    if(useFSG && transcribe)
    {
        Fatal<incompatible_parameters_exception>() << "FSG and Transcribing are not compatible!";
    }

    if(searchPhonemes && transcribe)
    {
        assert(!"Sorry, currently phoneme transcribing is not supported!");
    }

    if(verbosity)
    {
        Log.filter(Level::None);
        printParams();
    }
}

void Params::printParams() const noexcept
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
    std::cout<<"searchPhonemes      : "<<searchPhonemes<<"\n";
    std::cout<<"displayRecognised   : "<<displayRecognised<<"\n";
    std::cout<<"readStream          : "<<readStream<<"\n";
    std::cout<<"quickDict           : " <<quickDict<<"\n";
    std::cout<<"quickLM             : " <<quickLM<<"\n";

    std::cout<<"\n\n=====================================================\n\n";

}
