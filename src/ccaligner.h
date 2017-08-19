/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_CCALIGNER_H
#define CCALIGNER_CCALIGNER_H

#include "generate_approx_timestamp.h"
#include "recognize_using_pocketsphinx.h"

enum alignerType
{
    approxAligner,  //approximation based alignment
    none            //no aligner is specified
};

class CCAligner
{
    alignerType _useAligner;                            //type of aligner : (approx / word based /phoneme based etc.)
    outputFormats _outputFormat;                        //output format :  (xml/srt/json/stdout etc.)
    std::string _filename;                              //path to input file

public:

    CCAligner();
    bool alignerAlreadySet();                           //return true if type of aligner is already set
    void setAligner(alignerType useAligner);            //set the type of aligner to be used
    bool outputFormatAlreadySet();                      //return true if output format is already specified
    void setOutputFormat(outputFormats outputFormat);   //set output format
    bool inputFileNameAlreadySet();                     //return true if input file name is already set
    void setInputFileName(std::string fileName);        //set the name/path of input file

    int initAligner();                                  //initialize the aligner
    ~CCAligner();
};

void printUsage();                                      //print usage instruction (soon to be added inside class)


#endif //CCALIGNER_CCALIGNER_H
