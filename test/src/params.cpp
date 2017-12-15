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

    char* args[] = { "",
        "-wav", wavFilePath,
        "-srt", srtFilePath,
        "-out", outFilePath,
        "-oFormat", "json" };

    params.inputParams(sizeof(args) / sizeof(args[0]), args);
    
    ASSERT_EQ(params.audioFileName, wavFilePath);
    ASSERT_EQ(params.subtitleFileName, srtFilePath);
    ASSERT_EQ(params.outputFileName, outFilePath);
    ASSERT_EQ(params.outputFormat, json);

}
