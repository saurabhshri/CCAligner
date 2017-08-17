//
// Created by Saurabh Shrivastava on 17/08/17.
//

#ifndef CCALIGNER_OUTPUT_HANDLER_H
#define CCALIGNER_OUTPUT_HANDLER_H

#include "commons.h"
#include "srtparser.h"

inline bool initFile(std::string fileName, outputFormats outputFormat);
inline bool printFileEnd(std::string fileName, outputFormats outputFormat);

bool printSRT(std::string fileName, std::vector <SubtitleItem*> subtitles, outputOptions printOption);          //prints the aligned result in SRT format
inline int printSRTContinuous(std::string fileName, int subCount, SubtitleItem* sub, outputOptions printOption); //prints the aligned result in SRT format as they are generated

bool printJSON(std::string fileName, std::vector <SubtitleItem*> subtitles);    //prints the aligned result in JSON format
inline bool printJSONContinuous(std::string fileName, SubtitleItem* sub);        //prints the aligned result in JSON format as they are generated

bool printXML(std::string fileName, std::vector <SubtitleItem*> subtitles);     //prints the aligned information in XML format
inline bool printXMLContinuous(std::string fileName, SubtitleItem* sub);         //prints the aligned information in XML format as they are generated

bool printKaraoke(std::string fileName, std::vector <SubtitleItem*> subtitles, outputOptions printOption);          //prints the aligned information in Karaoke format
inline int printKaraokeContinuous(std::string fileName, int subCount, SubtitleItem* sub, outputOptions printOption); //prints the aligned information in Karaoke format as they are generated

#endif //CCALIGNER_OUTPUT_HANDLER_H
