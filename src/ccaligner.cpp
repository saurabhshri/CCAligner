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

CCAligner::CCAligner(Params * paramters)
{
    _parameters = paramters;
    should_log = _parameters->verbosity ;
}

int CCAligner::initAligner()
{
    if(_parameters->chosenAlignerType == approxAligner)
    {
        ApproxAligner * aligner = new ApproxAligner(_parameters->audioFileName,srt);
        aligner->align();
        delete aligner;
    }

    else if(_parameters->chosenAlignerType == asrAligner)
    {
        PocketsphinxAligner * aligner = new PocketsphinxAligner(_parameters->audioFileName, _parameters->subtitleFileName);
        aligner->generateGrammar(_parameters->grammarType);
        aligner->initDecoder(_parameters->modelPath, _parameters->lmPath, _parameters->dictPath, _parameters->fsgPath, _parameters->logPath);
        if(_parameters->transcribe)
        {
            aligner->transcribe();
        }

        else
        {
            if(_parameters->useFSG)
                aligner->alignWithFSG();
            else
                aligner->align(_parameters->printOption);
        }


        delete(aligner);
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
    Params *paramters = new Params();
    paramters->inputParams(argc,argv);

    CCAligner *ccaligner = new CCAligner(paramters);
    ccaligner->initAligner();

    delete(paramters);
    delete(ccaligner);

    return 0;
}
