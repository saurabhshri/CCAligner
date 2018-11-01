#include <gtest/gtest.h>
#include "../../src/lib_ccaligner/read_wav_file.h"

TEST(twoBytesToIntTest, RightIndex){
	ASSERT_EQ(twoBytesToInt([1, 1], 0), 257);
	ASSERT_EQ(twoBytesToInt([1, 2, 3, 4, 5], 1), 770);
	ASSERT_EQ(twoBytesToInt([1,2,3,4], 2), 1027);
}