/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#include <iostream>
#include "ccaligner.h"
using namespace std;

CCAligner::CCAligner(Params* parameters) : logFile(parameters->logPath)
{
    _parameters = parameters;
  
    Logger::Sink sink(logFile, false);
    sink.setMinimumOutputLevel(Logger::Level::verbose);
    getLogger().addSink(sink);

    Logger::Log<>(getLogger(), __FILE__, __FUNCTION__, __LINE__, Logger::Level::verbose);
}

CCAligner::~CCAligner() {
    logFile.close();
}

int CCAligner::initAligner()
{
    if(_parameters->chosenAlignerType == approxAligner)
    {
        ApproxAligner(_parameters->subtitleFileName, srt).align();
    }
    else if(_parameters->chosenAlignerType == asrAligner)
    {
        PocketsphinxAligner(_parameters).align();
    }
    else
    {
        FATAL(InvalidParameters) << "Unsupported Aligner Type!";
    }

    return 1;
}


