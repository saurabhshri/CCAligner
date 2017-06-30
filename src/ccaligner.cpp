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
    return 0;
}
