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
    std::string audioFileName, subtitleFileName, modelPath, lmPath, dictPath, fsgPath, logPath;
    unsigned long searchWindow, sampleWindow, audioWindow;
    alignerType chosenAlignerType;
    grammarName grammarType;
    outputOptions printOption;
    bool verbosity, useFSG, transcribe, useBatchMode, useExperimentalParams;

    Params();
    void inputParams(int argc, char *argv[]);
    void validateParams();
    ~Params();

};

#endif //CCALIGNER_PARAMS_H
