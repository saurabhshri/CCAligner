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

    constexpr const char* wavFilePath = "path/to/wav/file";
    constexpr const char* srtFilePath = "path/to/srt/file";
    constexpr const char* outFilePath = "path/to/out/file";
    constexpr const char* modelPath = "path/to/model/file";
    constexpr const char* lmPath = "path/to/lm/file";
    constexpr const char* dictPath = "path/to/dict/file";
    constexpr const char* fsgPath = "path/to/fsg/file";
    constexpr const char* logPath = "path/to/log/file";
    constexpr const char* phoneLMPath = "path/to/phoneLM/file";
    constexpr const char* alignerLogPath = "path/to/alignerLog/file";
    constexpr const char* phoneLogPath = "path/to/phoneLog/file";
    
    std::vector<std::string> args = { "",
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
    std::vector<char*> argv;
    for (auto& arg : args)
        argv.push_back(&arg[0]);
    argv.push_back(nullptr);

    params.inputParams(argv.size() - 1, argv.data());
    
    ASSERT_STREQ(params.audioFileName.c_str(), wavFilePath);
    ASSERT_STREQ(params.subtitleFileName.c_str(), srtFilePath);
    ASSERT_STREQ(params.outputFileName.c_str(), outFilePath);
    ASSERT_EQ(params.outputFormat, json);
    ASSERT_STREQ(params.modelPath.c_str(), modelPath);
    ASSERT_STREQ(params.lmPath.c_str(), lmPath);
    ASSERT_STREQ(params.dictPath.c_str(), dictPath);
    ASSERT_STREQ(params.fsgPath.c_str(), fsgPath);
    ASSERT_STREQ(params.logPath.c_str(), logPath);
    ASSERT_STREQ(params.phonemeLogPath.c_str(), phoneLogPath);
    ASSERT_STREQ(params.alignerLogPath.c_str(), alignerLogPath);
}
