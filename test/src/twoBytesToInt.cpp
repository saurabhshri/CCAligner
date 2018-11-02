#include <gtest/gtest.h>
#include "../../src/lib_ccaligner/read_wav_file.h"


// Trying to get index of empty vector
TEST(twoBytesToIntTest, EmptyVector){
	ASSERT_EQ(twoBytesToInt([], 0), -1);
	ASSERT_EQ(twoBytesToInt([], 1), -1);
	ASSERT_EQ(twoBytesToInt([], 10), -1);
}

// Trying to get index which isn't in possible range
TEST(twoBytesToIntTest, WrongIndex){
	ASSERT_EQ(twoBytesToInt([1, 1], 3), -1);
	ASSERT_EQ(twoBytesToInt([1], 1), -1);
	ASSERT_EQ(twoBytesToInt([1,2,3,4], 10), -1);
}

// Trying to get index which is in possible rang
// Processed input data manualy to get right results from real samples
TEST(twoBytesToIntTest, RightIndex){
	ASSERT_EQ(twoBytesToInt(['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' '], 20), 1);
	ASSERT_EQ(twoBytesToInt(['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' ','',' ','Ђ','>',' ',' ',' ','}',' ',' ','',' '], 22), 1);
	ASSERT_EQ(twoBytesToInt([['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' ','',' ','Ђ','>',' ',' ',' ','}',' ',' ','',' '], 32), 2);
}
