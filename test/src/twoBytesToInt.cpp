#include <gtest/gtest.h>
#include "../../src/lib_ccaligner/read_wav_file.h"


// Trying to get index of empty vector
TEST(twoBytesToIntTest, EmptyVector){
	ASSERT_EQ(twoBytesToInt([], 0), -1);
	ASSERT_EQ(twoBytesToInt([], 1), -1);
	ASSERT_EQ(twoBytesToInt([], 10), -1);
}

// Trying to get index which isn't in possible range
TEST(twoBytesToIntTest, InvalidIndex){
	ASSERT_EQ(twoBytesToInt(['W', 'Q'], 3), -1);
	ASSERT_EQ(twoBytesToInt(['O'], 1), -1);
	ASSERT_EQ(twoBytesToInt(['F','G','A','Q'], 10), -1);
}

// Trying to get index which is in possible range
// Processed input data manualy to get right results from real samples
TEST(twoBytesToIntTest, ValidIndex){
	int index = {20, 22, 32};
	int results_processed_manually = {1, 1, 2};
	ASSERT_EQ(twoBytesToInt(['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' '], index[0]), results_processed_manually[0]);
	ASSERT_EQ(twoBytesToInt(['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' ','',' ','Ђ','>',' ',' ',' ','}',' ',' ','',' '], index[1]), results_processed_manually[1]);
	ASSERT_EQ(twoBytesToInt([['R','I','F','F','l','/','',' ','W','A','V','E','f','m','t',' ','',' ',' ',' ','',' ','',' ','Ђ','>',' ',' ',' ','}',' ',' ','',' '], index[2]), results_processed_manually[2]);
}
