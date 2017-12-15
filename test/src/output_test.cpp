/*
* Author   : Harry Yu
* Email    : harryyunull@gmail.com
* Link     : https://github.com/harrynull
*/

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <fstream>
#include <json.hpp>
#include "../../src/lib_ccaligner/ccaligner.h"
using Json = nlohmann::json;

class OutputTest : public ::testing::TestWithParam<std::string> {
protected:
    std::string _wavFileName, _srtFileName, _outFileName;
    Params _params;
    // Will be called before each test is run.
    virtual void SetUp() {
        _wavFileName = GetParam() + ".wav";
        _srtFileName = GetParam() + ".srt";
        _outFileName = GetParam() + ".json";

        // Construct Params
        char* args[] = { "",
            "-wav", &_wavFileName[0],
            "-srt", &_srtFileName[0],
            "-out", &_outFileName[0],
            "-oFormat", "json",
            "-verbosity", "info"
        };
        _params.inputParams(sizeof(args)/sizeof(args[0]), args);
    }
};

TEST_P(OutputTest, TestOutput) {
    CCAligner(&_params).initAligner();
    std::ifstream ifs(_outFileName);
    if (!ifs) FAIL() << "No file is generated.";
    std::string generatedResult((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
    ifs.close();

    std::ifstream ifsEx(_outFileName + ".expected");
    if (!ifsEx) FAIL() << "Failed to read expected result. Test could be broken";
    std::string expectedResult((std::istreambuf_iterator<char>(ifsEx)),
        std::istreambuf_iterator<char>());
    ifsEx.close();
    // Compare the generated result with the expected result
    ASSERT_EQ(generatedResult, expectedResult);
}

// Return the paths of the wave files that are to be tested.
std::vector<std::string> getTestList() {
    std::cout << "Reading test_info.json..." << std::endl;
    std::ifstream infoStream("test_info.json");
    if (!infoStream) {
        std::cerr << "Failed to open test_info.json. Abort..." << std::endl;
        std::cerr << "You should run this program under test/ directory." << std::endl;
        std::exit(-1);
    }
    Json info = Json::parse( std::string(std::istreambuf_iterator<char>(infoStream),
        std::istreambuf_iterator<char>()) );
    if (!info.is_object()) {
        std::cerr << "test_info.json should contain a JSON object. Abort..." << std::endl;
        std::exit(-1);
    }
    return info["wav_tests"];
}

INSTANTIATE_TEST_CASE_P(WaveTests, OutputTest, ::testing::ValuesIn(getTestList()));
