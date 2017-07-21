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

public:
    Aligner(std::string inputAudioFileName, std::string inputSubtitleFileName);
    bool initDecoder(std::string modelPath, std::string lmPath, std::string dictPath, std::string fsgPath, std::string logPath);
    bool printWordTimes(cmd_ln_t *config, ps_decoder_t *ps);
    recognisedBlock findAndSetWordTimes(cmd_ln_t *config, ps_decoder_t *ps, SubtitleItem *sub);
    bool reInitDecoder(cmd_ln_t *config, ps_decoder_t *ps);
    bool generateGrammar();
    bool align();
    bool alignWithFSG();
    ~Aligner();

};

#endif //CCALIGNER_RECOGNIZE_USING_POCKETSPHINX_H
