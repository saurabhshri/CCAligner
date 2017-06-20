/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_CCALIGNER_H
#define CCALIGNER_CCALIGNER_H

#include "generate_approx_timestamp.h"

enum alignerType
{
    approxAligner,
    none
};

class CCAligner
{
    alignerType _useAligner;
    outputFormats _outputFormat;
    std::string _filename;

public:

    CCAligner();
    bool alignerAlreadySet();
    void setAligner(alignerType useAligner);
    bool outputFormatAlreadySet();
    void setOutputFormat(outputFormats outputFormat);
    bool inputFileNameAlreadySet();
    void setInputFileName(std::string fileName);

    int initAligner();
    ~CCAligner();
};

void printUsage();


#endif //CCALIGNER_CCALIGNER_H
