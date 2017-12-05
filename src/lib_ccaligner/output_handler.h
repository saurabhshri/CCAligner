/*
 * Author   : Saurabh Shrivastava
 * Email    : saurabh.shrivastava54@gmail.com
 * Link     : https://github.com/saurabhshri
 */

#ifndef CCALIGNER_OUTPUT_HANDLER_H
#define CCALIGNER_OUTPUT_HANDLER_H

#include "commons.h"
#include "srtparser.h"

void initFile(const std::string& fileName, outputFormats outputFormat);
void printFileEnd(const std::string& fileName, outputFormats outputFormat);

void printTranscriptionHeader(const std::string& fileName, outputFormats outputFormat);
void printTranscriptionFooter(const std::string& fileName, outputFormats outputFormat);


void printSRT(const std::string& fileName, std::vector <SubtitleItem*> subtitles, outputOptions printOption);          //prints the aligned result in SRT format
int printSRTContinuous(const std::string& fileName, int subCount, SubtitleItem* sub, outputOptions printOption); //prints the aligned result in SRT format as they are generated
void printTranscriptionAsSRTContinuous(const std::string& fileName, AlignedData *alignedData, int printedTillIndex);        //prints the transcribed result in JSON format as they are generated

void printJSON(const std::string& fileName, std::vector <SubtitleItem*> subtitles);    //prints the aligned result in JSON format
void printJSONContinuous(const std::string& fileName, SubtitleItem* sub);        //prints the aligned result in JSON format as they are generated
void printTranscriptionAsJSONContinuous(const std::string& fileName, AlignedData *alignedData, int printedTillIndex);        //prints the transcribed result in JSON format as they are generated

void printXML(const std::string& fileName, std::vector <SubtitleItem*> subtitles);     //prints the aligned information in XML format
void printXMLContinuous(const std::string& fileName, SubtitleItem* sub);         //prints the aligned information in XML format as they are generated
void printTranscriptionAsXMLContinuous(const std::string& fileName, AlignedData *alignedData, int printedTillIndex);         //prints the transcribed information in XML format as they are generated

void printKaraoke(const std::string& fileName, std::vector <SubtitleItem*> subtitles, outputOptions printOption);          //prints the aligned information in Karaoke format
int printKaraokeContinuous(const std::string& fileName, int subCount, SubtitleItem* sub, outputOptions printOption); //prints the aligned information in Karaoke format as they are generated

#endif //CCALIGNER_OUTPUT_HANDLER_H
