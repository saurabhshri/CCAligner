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
        case 228: return "EY"; //L'ä'
        case 226: return "AE"; //L'â'
	case 235: return "IY"; // L'ë'
	case 234: return "EH"; // L'ê'
        case 239: return "AY"; //L'ï'
        case 238: return "IH"; //L'î'
        case 246: return "OW"; //L'ö'
        case 244: return "AA"; //L'ô'// could also be AO/AH
        case 252: return "UW"; //L'ü' // really Y+UW
        case 251: return "AH"; //L'û' // [ʌ] as in b[u]t
        case 117: return "UW"; //L'u'
        case 242: return "AO"; //L'ò'
        case 249: return "UH"; //L'ù'
        case 64: return "AH"; //L'@' // [ə] as in [a]lone
        case 934: return "AW"; // L'Ω'
        case 969: return "OY"; //L'ω'
        case 121: return "Y"; //L'y'
        case 119: return "W"; //L'w'
        case 605: return "ER"; //L'ɝ'
        case 112: return "P"; //L'p'
        case 98: return "B"; //L'b'
        case 116: return "T"; //L't'
        case 100: return "D"; //L'd'
        case 103: return "G"; //L'g'
        case 107: return "K"; //L'k'
        case 109: return "M"; //L'm'
        case 110: return "N"; //L'n'
        case 241: return "NG"; //L'ñ'
        case 102: return "F"; //L'f'
        case 118: return "V"; //L'v'
        case 43: return "TH"; // L'+'// also covers DH
        case 115: return "S"; //L's'
        case 122: return "Z"; //L'z'
        case 36: return "SH"; //L'$' // also covers ZH
        case 231: return "CH"; //L'ç'
        case 106: return "JH"; //L'j'
        case 114: return "R"; //L'r'
        case 108: return "L"; //L'l'
        case 104: return "HH"; //L'h'
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
            DEBUG << "Error converting " << phone;
        }

        if (phone != lastPhoneme)
        {
            result.push_back(phone);
        }

        lastPhoneme = phone;
    }
    return result;
}

