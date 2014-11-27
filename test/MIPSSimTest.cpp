#include "gtest/gtest.h"
#include "../src/MIPSSim.h"

TEST(MIPSSim, testReadInput) {
}

TEST(MIPSSim, testIsSimulation) {
}

TEST(MIPSSim, testIsDisAssembly) {
	Arguments input;
	input.operation = DIS;

	EXPECT_TRUE(isDisAssembly(input));
}

