//
// Created by Saurabh Shrivastava on 17/08/17.
//

#ifndef CCALIGNER_OUTPUT_HANDLER_H
#define CCALIGNER_OUTPUT_HANDLER_H

#include "commons.h"
#include "srtparser.h"

bool initFile(std::string fileName, outputFormats format);
bool printSRT(std::string fileName, std::vector <SubtitleItem*> subtitles, outputOptions printOption); //prints the aligned result in SRT format
bool printJSON(std::string fileName, std::vector <SubtitleItem*> subtitles);                           //prints the aligned result in JSON format
bool printXML(std::string fileName, std::vector <SubtitleItem*> subtitles);                            //prints the aligned information in XML format
bool printKaraoke(std::string fileName, std::vector <SubtitleItem*> subtitles, outputOptions printOption); //prints the aligned information in Karaoke format

#endif //CCALIGNER_OUTPUT_HANDLER_H
