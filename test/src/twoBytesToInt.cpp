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

// Trying to get index which is in possible range
TEST(twoBytesToIntTest, RightIndex){
	ASSERT_NE(twoBytesToInt([1, 1], 0), -1);
	ASSERT_NE(twoBytesToInt([1, 2, 3, 4, 5], 1), -1);
	ASSERT_NE(twoBytesToInt([1,2,3,4], 2), -1);
}
