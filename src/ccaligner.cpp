/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include "ccaligner.h"

void printUsage()
{
    std::cout<<"\n\nUsage : \n"
          "ApproxAligner : ccaligner -a input.srt\n"
          "                ccaligner -a input.srt -of <output_format>"
          "                                       (srt/xml/json)"
          "           e.g. ccaligner -a input.srt -of xml"
          ""
          "NOTE : This tool is in it's early stage and is constantly evolving; things are likely to change!";
}

CCAligner::CCAligner()
{
    _useAligner = none;
    _outputFormat = blank;
}

void CCAligner::setAligner(alignerType useAligner)
{
    _useAligner = useAligner;
}

bool CCAligner::alignerAlreadySet()
{
    return _useAligner != none;
}

void CCAligner::setOutputFormat(outputFormats outputFormat)
{
    _outputFormat = outputFormat;
}

bool CCAligner::outputFormatAlreadySet()
{
    return _outputFormat != blank;
}

void CCAligner::setInputFileName(std::string fileName)
{
    _filename = fileName;
}

bool CCAligner::inputFileNameAlreadySet()
{
    return !_filename.empty();
}

int CCAligner::initAligner()
{
    if(_useAligner == approxAligner)
    {
        if(!outputFormatAlreadySet())
        {
            _outputFormat = srt;        //by default output format is SRT
        }

        ApproxAligner * aligner = new ApproxAligner(_filename,_outputFormat);
        aligner->align();
        delete aligner;
    }

    else
    {
        std::cout<<"Support for more robust aligners is coming soon! Stay tuned!\n";
        exit(3);
    }

    return 1;
}

CCAligner::~CCAligner()
{

}

int main(int argc, char *argv[])
{
    //default values :

    std::string modelPath ("model"), lmPath ("tempFiles/lm/complete.lm"), dictPath ("tempFiles/dict/complete.dict"), fsgPath ("tempFiles/fsg/"), logPath ("tempFiles/log.log");
    int generateGrammar = 1;    //1 = print, 0 = don't print, 2 = print corpus for web tool
    int printSubtile = 1;       //1 = print both, 2 = print only aligned, 3 = show colour diff
    bool useFSG = false;


    std::string audioFileName, subtitleFileName;

    for(int i=1;i<argc;i++)         //parsing arguments
    {
        std::string param(argv[i]);

        if(param == "-model")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            modelPath = argv[i+1];
            i++;
        }

        if(param == "-lm")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            lmPath = argv[i+1];
            i++;
        }


        if(param == "-dict")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            dictPath = argv[i+1];
            i++;
        }


        if(param == "-fsg")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            fsgPath = argv[i+1];
            i++;
        }


        if(param == "-log")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            logPath = argv[i+1];
            i++;
        }

        if(param == "--generate-grammar")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            std::string subParam (argv[i+1]);

            if(subParam == "true")
                generateGrammar = 1;

            else if(subParam == "false")
                generateGrammar = 0;

            else if(subParam == "printCorpus")
                generateGrammar = 2;

            else
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            i++;
        }

        if(param == "-in")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            audioFileName = argv[i+1];
            i++;
        }

        if(param == "-srt")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            subtitleFileName = argv[i+1];
            i++;
        }

        if(param == "--print-aligned")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            std::string subParam (argv[i+1]);

            if(subParam == "color")
                printSubtile = 3;

            else if(subParam == "true")
                printSubtile = 2;

            else
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            i++;
        }

        if(param == "--use-fsg")
        {
            if(i+1 > argc)
            {
                std::cout<<"Incorrect parameters, parsing failed!";
                exit(2);
            }

            std::string subParam (argv[i+1]);

            if(subParam == "yes")
                useFSG = true;

            i++;
        }

    }


    if(audioFileName.empty() || subtitleFileName.empty())
    {
        std::cout<<"Filenames not supplied!";
        exit(2);
    }

    Aligner * aligner = new Aligner(audioFileName, subtitleFileName);

    if(generateGrammar == 1)
        aligner->generateGrammar(all);

    else if(generateGrammar == 2)
    {
        aligner->generateGrammar(corpus);
        exit(0);
    }

    aligner->initDecoder(modelPath, lmPath, dictPath, fsgPath, logPath);

    if(useFSG)
        aligner->alignWithFSG();
    else
        aligner->align(printSubtile);

    delete(aligner);

    /*
    CCAligner *ccaligner = new CCAligner();
    ccaligner->setAligner(approxAligner);
    ccaligner->setOutputFormat(console);
    ccaligner->setInputFileName(argv[2]);
    ccaligner->initAligner();


    if(argc<2)
    {
        printUsage();
        return 1;
    }

    CCAligner *ccaligner = new CCAligner();

    for(int i=1;i<argc;i++)         //parsing arguments ; TODO: Do this separately
    {
        std::string param(argv[i]);

        if(param == "-a")
        {
            if(ccaligner->alignerAlreadySet())
            {
                std::cout<<"Error : More than one Aligner type specified!\n";
                printUsage();
                exit(2);
            }

            ccaligner->setAligner(approxAligner);
        }

        else if(param == "-of")
        {
            if(i + 1 >= argc)
            {
                std::cout<<"Error : -of must be followed by output format!\n";
                printUsage();
                exit(2);
            }

            if(ccaligner->outputFormatAlreadySet())
            {
                std::cout<<"Error : More than one output format specified!\n";
                printUsage();
                exit(2);
            }

            std::string subParam(argv[i+1]);

            if(subParam == "SRT" || subParam =="srt")
                ccaligner->setOutputFormat(srt);

            else if(subParam == "XML" || subParam =="xml")
                ccaligner->setOutputFormat(xml);

            else if(subParam == "JSON" || subParam =="json")
                ccaligner->setOutputFormat(json);

            else if(subParam == "stdout" || subParam =="console")
                ccaligner->setOutputFormat(console);
            else
            {
                std::cout<<"Error : Incorrect output format specified : "<<subParam;
                printUsage();
                exit(2);
            }

            i++;
        }

        else
        {
            if(ccaligner->inputFileNameAlreadySet())
            {
                std::cout<<"Error : Input filename already set!";
                printUsage();
                exit(2);
            }
            ccaligner->setInputFileName(param);
        }
    }

    ccaligner->initAligner();   //initialise the aligner as per the arguments.

    delete ccaligner;
    */

    return 0;
}
