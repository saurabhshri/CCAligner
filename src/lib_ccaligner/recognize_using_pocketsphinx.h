/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_RECOGNIZE_USING_POCKETSPHINX_H
#define CCALIGNER_RECOGNIZE_USING_POCKETSPHINX_H

#include "srtparser.h"
#include "read_wav_file.h"
#include "pocketsphinx.h"
#include "grammar_tools.h"
#include "generate_approx_timestamp.h"

int levenshtein_distance(const std::string& firstWord, const std::string& secondWord);

class Aligner
{
private:
    std::string _audioFileName,_subtitleFileName;          //input and output filenames
    std::vector<int16_t> _samples;
    std::vector <SubtitleItem*> _subtitles;

    std::string _modelPath, _lmPath, _dictPath, _fsgPath, _logPath;

    ps_decoder_t * _ps;
    cmd_ln_t * _config;
    char const * _hyp;
    int _rv;
    int32 _score;

    bool printWordTimes(cmd_ln_t *config, ps_decoder_t *ps);
    bool printRecognisedWordAsSRT(cmd_ln_t *config, ps_decoder_t *ps);
    recognisedBlock findAndSetWordTimes(cmd_ln_t *config, ps_decoder_t *ps, SubtitleItem *sub);
    bool reInitDecoder(cmd_ln_t *config, ps_decoder_t *ps);

public:
    Aligner(std::string inputAudioFileName, std::string inputSubtitleFileName);
    bool initDecoder(std::string modelPath, std::string lmPath, std::string dictPath, std::string fsgPath, std::string logPath);
    bool generateGrammar(grammarName name);
    bool align(srtOptions printOption);
    bool alignWithFSG();
    bool transcribe();
    ~Aligner();

};

#endif //CCALIGNER_RECOGNIZE_USING_POCKETSPHINX_H
