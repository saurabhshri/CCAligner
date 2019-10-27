#include <gtest/gtest.h>
#include "../../src/lib_ccaligner/read_wav_file.h"


// Empty fileData
TEST(checkValidWave, EmptyVector){
    ASSERT_NE(checkValidWave([]), true);
}

// Not enough fileData
TEST(checkValidWave, NotFullVector){
    ASSERT_NE(checkValidWave(['R']), true);
    ASSERT_NE(checkValidWave(['R', 'I']), true);
    ASSERT_NE(checkValidWave(['R', 'I', 'F']), true);
    ASSERT_NE(checkValidWave(['R', 'I', 'F', 'F']), true);
    ASSERT_NE(checkValidWave(['W']), true);
    ASSERT_NE(checkValidWave(['W', 'A']), true);
    ASSERT_NE(checkValidWave(['W', 'A', 'V']), true);
    ASSERT_NE(checkValidWave(['W', 'A', 'V', 'E']), true);
}

// Normal fileData

TEST(checkValidWave, FullVector){
    ASSERT_EQ(checkValidWave(['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' ']), true);
    ASSERT_EQ(checkValidWave(['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' ','',' ','Ђ','>',' ',' ',' ','}',' ',' ','',' ']), true);
}
