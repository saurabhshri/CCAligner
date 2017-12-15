/*
* Author   : Harry Yu
* Email    : harryyunull@gmail.com
* Link     : https://github.com/harrynull
*/

#include <gtest/gtest.h>
#include <string>
#include "../../src/lib_ccaligner/params.h"

TEST(Params, Arguments) {
    Params params;

    constexpr char* wavFilePath = "path/to/wav/file";
    constexpr char* srtFilePath = "path/to/srt/file";
    constexpr char* outFilePath = "path/to/out/file";
    constexpr char* modelPath = "path/to/model/file";
    constexpr char* lmPath = "path/to/lm/file";
    constexpr char* dictPath = "path/to/dict/file";
    constexpr char* fsgPath = "path/to/fsg/file";
    constexpr char* logPath = "path/to/log/file";
    constexpr char* phoneLMPath = "path/to/phoneLM/file";
    constexpr char* alignerLogPath = "path/to/alignerLog/file";
    constexpr char* phoneLogPath = "path/to/phoneLog/file";
    
    char* args[] = { "",
        "-wav", wavFilePath,
        "-srt", srtFilePath,
        "-out", outFilePath,
        "-model", modelPath,
        "-lm", lmPath,
        "-dict", dictPath,
        "-fsg", fsgPath,
        "-log", logPath,
        "-phoneLM", phoneLMPath,
        "-alignerLog", alignerLogPath,
        "-phoneLog", phoneLogPath,
        "-oFormat", "json" };

    params.inputParams(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(params.audioFileName, wavFilePath);
    ASSERT_EQ(params.subtitleFileName, srtFilePath);
    ASSERT_EQ(params.outputFileName, outFilePath);
    ASSERT_EQ(params.outputFormat, json);
    ASSERT_EQ(params.modelPath, modelPath);
    ASSERT_EQ(params.lmPath, lmPath);
    ASSERT_EQ(params.dictPath, dictPath);
    ASSERT_EQ(params.fsgPath, fsgPath);
    ASSERT_EQ(params.logPath, logPath);
    ASSERT_EQ(params.phonemeLogPath, phoneLogPath);
    ASSERT_EQ(params.alignerLogPath, alignerLogPath);
}
