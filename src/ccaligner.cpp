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
    _paramters = paramters;
}

int CCAligner::initAligner()
{
    if(_paramters->chosenAlignerType == approxAligner)
    {
        ApproxAligner * aligner = new ApproxAligner(_paramters->audioFileName,srt);
        aligner->align();
        delete aligner;
    }

    else if(_paramters->chosenAlignerType == asrAligner)
    {
        PocketsphinxAligner * aligner = new PocketsphinxAligner(_paramters->audioFileName, _paramters->subtitleFileName);
        aligner->generateGrammar(_paramters->grammarType);
        aligner->initDecoder(_paramters->modelPath, _paramters->lmPath, _paramters->dictPath, _paramters->fsgPath, _paramters->logPath);
        if(_paramters->transcribe)
        {
            aligner->transcribe();
        }

        else
        {
            if(_paramters->useFSG)
                aligner->alignWithFSG();
            else
                aligner->align(_paramters->printOption);
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
