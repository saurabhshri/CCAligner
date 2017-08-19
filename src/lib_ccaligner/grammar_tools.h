/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_GRAMMAR_TOOLS_H
#define CCALIGNER_GRAMMAR_TOOLS_H

#include "srtparser.h"

enum grammarName      //various possible output formats
{
    corpus,
    fsg,
    lm,
    dict,
    vocab,
    all              //means no output format is specified
};

std::string StringToLower(std::string strToConvert);
bool generate(std::vector <SubtitleItem*> subtitles, grammarName name = all);


#endif //CCALIGNER_GRAMMAR_TOOLS_H
