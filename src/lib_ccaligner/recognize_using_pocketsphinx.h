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
#include "commons.h"
#include "params.h"
#include "output_handler.h"

int levenshtein_distance(const std::string& firstWord, const std::string& secondWord);

class PocketsphinxAligner
{
private:
    std::string _audioFileName, _subtitleFileName, _outputFileName;          //input and output filenames

    std::unique_ptr<WaveFileData> _file;
    SubtitleParserFactory _subParserFactory;
    SubtitleParser * _parser;
    std::vector <SubtitleItem*> _subtitles;
    std::vector<int16_t> _samples;

    AlignedData _alignedData;
    Params* _parameters;

    std::string _modelPath, _lmPath, _dictPath, _fsgPath, _logPath, _phoneticlmPath, _phonemeLogPath;
    long int _audioWindow, _sampleWindow, _searchWindow;

    ps_decoder_t * _psWordDecoder, * _psPhonemeDecoder;
    cmd_ln_t * _configWord, * _configPhoneme;
    char const * _hypWord, * _hypPhoneme;
    int _rvWord, _rvPhoneme;
    int32 _scoreWord, _scorePhoneme;

    bool printWordTimes(cmd_ln_t *config, ps_decoder_t *ps);
    int findTranscribedWordTimings(cmd_ln_t *config, ps_decoder_t *ps, int index);
    recognisedBlock findAndSetWordTimes(cmd_ln_t *config, ps_decoder_t *ps, SubtitleItem *sub);
    bool findAndSetPhonemeTimes(cmd_ln_t *config, ps_decoder_t *ps, SubtitleItem *sub);
    bool reInitDecoder(cmd_ln_t *config, ps_decoder_t *ps);
    bool initPhonemeDecoder(const std::string& phoneticlmPath, const std::string& phonemeLogPath);

public:
    PocketsphinxAligner(Params* parameters) noexcept;
    bool initDecoder(const std::string& modelPath, const std::string& lmPath, const std::string& dictPath, const std::string& fsgPath, const std::string& logPath);
    bool generateGrammar(grammarName name);
    bool recognise();
    bool alignWithFSG();
    bool align();
    bool recognisePhonemes(const int16_t *sample, int readLimit, SubtitleItem *sub);
    bool transcribe();
    bool printAligned(const std::string& outputFileName, outputFormats format) const noexcept;
    ~PocketsphinxAligner();

};

#endif //CCALIGNER_RECOGNIZE_USING_POCKETSPHINX_H
