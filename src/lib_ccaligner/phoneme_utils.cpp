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

#include "phoneme_utils.h"

std::wstring latin1ToWide(const std::string& s)
{
    std::wstring result;

    for (unsigned char c : s)
    {
        result.append(1, c);
    }

    return result;
}

const std::vector<std::pair<std::wregex, std::wstring>>& getReplacementRules()
{
    static std::vector<std::pair<std::wregex, std::wstring>> rules
        {
        #include "g2p_rules.cpp"

            // Turn bigrams into unigrams for easier conversion
            { std::wregex(L"ôw"), L"Ω" },
            { std::wregex(L"öy"), L"ω" },
            { std::wregex(L"@r"), L"ɝ" }
        };

    return rules;
}

Phoneme charToPhone(wchar_t c)
{
    // For reference, see http://www.zompist.com/spell.html

    switch (c)
    {
        case L'ä': return "EY";
        case L'â': return "AE";
        case L'ë': return "IY";
        case L'ê': return "EH";
        case L'ï': return "AY";
        case L'î': return "IH";
        case L'ö': return "OW";
        case L'ô': return "AA"; // could also be AO/AH
        case L'ü': return "UW"; // really Y+UW
        case L'û': return "AH"; // [ʌ] as in b[u]t
        case L'u': return "UW";
        case L'ò': return "AO";
        case L'ù': return "UH";
        case L'@': return "AH"; // [ə] as in [a]lone
        case L'Ω': return "AW";
        case L'ω': return "OY";
        case L'y': return "Y";
        case L'w': return "W";
        case L'ɝ': return "ER";
        case L'p': return "P";
        case L'b': return "B";
        case L't': return "T";
        case L'd': return "D";
        case L'g': return "G";
        case L'k': return "K";
        case L'm': return "M";
        case L'n': return "N";
        case L'ñ': return "NG";
        case L'f': return "F";
        case L'v': return "V";
        case L'+': return "TH"; // also covers DH
        case L's': return "S";
        case L'z': return "Z";
        case L'$': return "SH"; // also covers ZH
        case L'ç': return "CH";
        case L'j': return "JH";
        case L'r': return "R";
        case L'l': return "L";
        case L'h': return "HH";
    }

    return " "; // treating noise as silence
}

std::vector<Phoneme> stringToPhoneme(const std::string &word)
{

    std::wstring wideWord = latin1ToWide(word);
    for (const auto& rule : getReplacementRules())
    {
        const std::wregex& regex = rule.first;
        const std::wstring& replacement = rule.second;

        // Repeatedly apply rule until there is no more change
        bool changed;
        do
        {
            std::wstring tmp = regex_replace(wideWord, regex, replacement);
            changed = tmp != wideWord;
            wideWord = tmp;

        } while (changed);
    }

    // Remove duplicate phones
    std::vector<Phoneme> result;
    Phoneme lastPhoneme = "Noise";

    for (wchar_t c : wideWord)
    {
        Phoneme phone = charToPhone(c);
        if (phone == "Noise")
        {
            std::cout<<"Error converting "<<phone<<"\n";
        }

        if (phone != lastPhoneme)
        {
            result.push_back(phone);
        }

        lastPhoneme = phone;
    }
    return result;
}

