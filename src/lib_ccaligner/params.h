/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 */

#ifndef CCALIGNER_PARAMS_H
#define CCALIGNER_PARAMS_H

#include "commons.h"

class Params
{
public:
    std::string audioFileName, subtitleFileName, outputFileName, modelPath, lmPath, dictPath, fsgPath, logPath, phoneticlmPath, phonemeLogPath;
    unsigned long searchWindow, sampleWindow, audioWindow;
    alignerType chosenAlignerType;
    grammarName grammarType;
    outputFormats outputFormat;
    outputOptions printOption;
    bool verbosity, useFSG, transcribe, useBatchMode, useExperimentalParams, searchPhonemes, displayRecognised, readStream, quickDict, quickLM;

    Params();
    void inputParams(int argc, char *argv[]);
    void validateParams();
    void printParams();
    ~Params();

};

#endif //CCALIGNER_PARAMS_H
