/*
* Author   : Saurabh Shrivastava
* Email    : saurabh.shrivastava54@gmail.com
* Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_PARAMS_H
#define CCALIGNER_PARAMS_H

#include "commons.h"

#include <ctime>

class Params {
    std::string localTime;
    void validateParams();
public:
    std::string audioFileName, subtitleFileName, transcriptFileName, outputFileName, modelPath, lmPath, dictPath, fsgPath, logPath, phoneticLmPath, phonemeLogPath, alignerLogPath;
    bool audioIsRaw;
    unsigned long searchWindow, sampleWindow, audioWindow;
    alignerType chosenAlignerType;
    grammarName grammarType;
    outputFormats outputFormat;
    outputOptions printOption;
    bool verbosity, usingTranscript, useFSG, transcribe, useBatchMode, useExperimentalParams, searchPhonemes, displayRecognised, readStream, quickDict, quickLM;

    Params() noexcept;
    void inputParams(int argc, char *argv[]);
    void printParams() const noexcept;

};

#endif //CCALIGNER_PARAMS_H
