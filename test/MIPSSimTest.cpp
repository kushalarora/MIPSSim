#include "gtest/gtest.h"
#include "../src/MIPSSim.h"
TEST(MIPSSim, testReadInput) {
}

TEST(MIPSSim, testIsSimulation) {
}

TEST(MIPSSim, testIsDisAssembly) {
    Input input;
    input.operation = DIS;

    EXPECT_TRUE(isDisAssembly(input));
}


