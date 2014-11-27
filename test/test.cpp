#include "gtest/gtest.h"

using namespace std;

TEST(doNothing, doNothing1) {
	EXPECT_TRUE(true);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
