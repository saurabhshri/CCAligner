#include <gtest/gtest.h>
#include "../../src/lib_ccaligner/read_wav_file.h"

TEST(twoBytesToIntTest, EmptyVector){
	ASSERT_EQ(twoBytesToInt([], 0), -1);
	ASSERT_EQ(twoBytesToInt([], 1), -1);
	ASSERT_EQ(twoBytesToInt([], 10), -1);
}


TEST(twoBytesToIntTest, WrongIndex){
	ASSERT_EQ(twoBytesToInt([1, 1], 3), -1);
	ASSERT_EQ(twoBytesToInt([1], 1), -1);
	ASSERT_EQ(twoBytesToInt([1,2,3,4], 10), -1);
}

TEST(twoBytesToIntTest, RightIndex){
	ASSERT_NE(twoBytesToInt([1, 1], 0), -1);
	ASSERT_NE(twoBytesToInt([1, 2, 3, 4, 5], 1), -1);
	ASSERT_NE(twoBytesToInt([1,2,3,4], 10), -1);
}