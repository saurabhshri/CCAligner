/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 *
 * Based on grapheme to phoneme conversion in Rhubarb Lip Sync (MIT-License)
 * Author   : Daniel S Wolf (https://github.com/DanielSWolf)
 * Link     : https://github.com/DanielSWolf/rhubarb-lip-sync
 *
 * Which was originally based on Sound Change Applier (MIT-License)
 * Author   : Mark Rosenfelder
 * Link     : http://www.zompist.com/sounds.htm
 */

#ifndef CCALIGNER_PHONEME_UTILS_H
#define CCALIGNER_PHONEME_UTILS_H

#include "commons.h"
using Phoneme = std::string;

std::wstring latin1ToWide(const std::string& s);
const std::vector<std::pair<std::wregex, std::wstring>>& getReplacementRules();
Phoneme charToPhone(wchar_t c);
std::vector<Phoneme> stringToPhoneme(const std::string &word);

#endif //CCALIGNER_PHONEME_UTILS_H
