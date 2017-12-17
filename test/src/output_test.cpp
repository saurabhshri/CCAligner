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

bool testListLoadSuccessfully = false;

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
        std::vector<std::string> args = { "",
            "-wav", _wavFileName,
            "-srt", _srtFileName,
            "-out", _outFileName,
            "-oFormat", "json",
            "-verbosity", "info",
            "--display-recognised", "no"
        };
        std::vector<char*> argv;
        for (auto& arg : args)
            argv.push_back(&arg[0]);
        argv.push_back(nullptr);

        _params.inputParams(argv.size() - 1, argv.data());
    }
};

TEST_P(OutputTest, TestOutput) {
    try{
        CCAligner(&_params).initAligner();
    }
    catch (std::runtime_error& exception) {
        FAIL() << "Exception occurred: " << exception.what();
    }
    std::ifstream ifs(_outFileName);
    if (!ifs) FAIL() << "No file is generated.";
    Json generatedResult = Json::parse(std::string(std::istreambuf_iterator<char>(ifs),
        std::istreambuf_iterator<char>()));
    ifs.close();

    std::ifstream ifsEx(_outFileName + ".expected");
    if (!ifsEx) FAIL() << "Failed to read expected result. Test could be broken";
    Json expectedResult = Json::parse(std::string(std::istreambuf_iterator<char>(ifsEx),
        std::istreambuf_iterator<char>()));
    ifsEx.close();
    // Compare the generated result with the expected result
    ASSERT_EQ(generatedResult, expectedResult);
}

TEST(WaveTests, NoTestSkipped) {
    // To ensure that tests won't pass when some tests are skipped.
    if (!testListLoadSuccessfully) FAIL() << "Some tests ares skipped.";
}

// Return the paths of the wave files that are to be tested.
std::vector<std::string> getTestList() {
    std::cout << "Reading test_info.json..." << std::endl;
    std::ifstream infoStream("test_info.json");
    if (!infoStream) {
        std::cerr << "Failed to open test_info.json. Some tests will be skipped..." << std::endl;
        std::cerr << "You should run this program under test/ directory." << std::endl;
        return {};
    }
    Json info = Json::parse(std::string(std::istreambuf_iterator<char>(infoStream),
        std::istreambuf_iterator<char>()) );
    if (!info.is_object()) {
        std::cerr << "test_info.json should contain a JSON object. Some tests will be skipped..." << std::endl;
        return {};
    }
    testListLoadSuccessfully = true;
    return info["wav_tests"];
}

INSTANTIATE_TEST_CASE_P(WaveTests, OutputTest, ::testing::ValuesIn(getTestList()));
