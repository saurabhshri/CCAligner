/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
*/

#ifndef CCALIGNER_GRAMMAR_TOOLS_H
#define CCALIGNER_GRAMMAR_TOOLS_H

#include "srtparser.h"
#include "commons.h"
#include "phoneme_utils.h"

bool generate(std::vector <SubtitleItem*> subtitles, grammarName name = complete_grammar);
bool generate(std::string transcriptFileName, grammarName name = complete_grammar);
void ConfigureQuickGenerationOptions(bool &generateQuickDict, bool &generateQuickLM, grammarName &name);
void CreateTempDirectories();
void CreateNewGrammarFiles(grammarName name, std::ofstream &corpusDump, std::ofstream &fsgDump,
	std::ofstream &vocabDump, std::ofstream &dictDump, std::ofstream &phoneticCorpusDump, std::ofstream &logDump);
void CreateBiasedLM(grammarName name, bool generateQuickLM);
void GenerateDict(bool generateQuickDict);
std::string getFileData(std::string _fileName);

#endif //CCALIGNER_GRAMMAR_TOOLS_H
