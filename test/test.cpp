#include "gtest/gtest.h"
#include<string>
#include "pipeline/ROBTest.cpp"
#include "pipeline/ResStationTest.cpp"
#include "InstFetchTest.cpp"
#include "DecodeTest.cpp"
#include "ExecuteTest.cpp"
#include "WriteResultTest.cpp"
#include "CommitTest.cpp"

using namespace std;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
