/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 */

#ifndef CCALIGNER_CCALIGNER_H
#define CCALIGNER_CCALIGNER_H

#include "params.h"
#include "recognize_using_pocketsphinx.h"

class CCAligner
{
    Params * _parameters;

public:

    CCAligner(Params *parameters);
    int initAligner();                                  //initialize the aligner
    ~CCAligner() = default;
};

void printUsage();                                      //print usage instruction (soon to be added inside class)


#endif //CCALIGNER_CCALIGNER_H
