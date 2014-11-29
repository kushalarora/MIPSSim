#include "../../src/pipeline/ReservationStation.h"
#include "../../src/simulator/Executor.h"
#include "../../src/instructions/InstructionBuilder.cpp"
#include<map>
#include<vector>
#include "gtest/gtest.h"

class ResStationTest : public ::testing::Test {
public:
	vector<Instruction*> insts;
    CDB cdb;
    Registers registers;
    map<unsigned int, int> SWAddToCount;
    RegisterStatus registerStatus;
    ReservationStation* resStation;

	ResStationTest() {
        // ADDI R8, R0, #44
		insts.push_back(InstructionBuilder::build(600, "00100000000010000000000000101100", 1));
        // LW R2, 716(R4)
		insts.push_back(InstructionBuilder::build(604, "10001100100000100000001011001100", 1));
        // SW R5, 716(R10)
		insts.push_back(InstructionBuilder::build(608, "10101101010001010000001011001100", 1));

        resStation = new ReservationStation(&cdb, &registerStatus, SWAddToCount, &registers);
	}
};

TEST_F(ResStationTest, testEmpty) {
    for (int i = 0; i < 9; i++) {
        resStation->add(insts[0]);
    }
    EXPECT_FALSE(resStation->isFull());

    resStation->add(insts[1]);
    EXPECT_TRUE(resStation->isFull());
}

TEST_F(ResStationTest, testRSEntry) {
    registers.set(0, 10);
    RSEntry* entry = resStation->add(insts[0]);

    EXPECT_EQ(RSEntry::DEFAULT_Q, entry->getQj());
    EXPECT_EQ(RSEntry::DEFAULT_Q, entry->getQk());

    EXPECT_EQ(10, entry->getVj());


    // Testing SW
    registers.set(10, 11);
    registers.set(5, 20);
    RSEntry* entry2 = resStation->add(insts[2]);


    EXPECT_EQ(RSEntry::DEFAULT_Q, entry2->getQj());
    EXPECT_EQ(RSEntry::DEFAULT_Q, entry2->getQk());


    EXPECT_EQ(11, entry2->getVj());
    EXPECT_EQ(20, entry2->getVk());


    // Testing conflict
    registerStatus.set(10, 3);
    registerStatus.set(5, 6);

    RSEntry* entry3 = resStation->add(insts[2]);

    EXPECT_EQ(3, entry3->getQj());
    EXPECT_EQ(6, entry3->getQk());


    EXPECT_EQ(RSEntry::DEFAULT_VALUE, entry3->getVj());
    EXPECT_EQ(RSEntry::DEFAULT_VALUE, entry3->getVk());
}


TEST_F(ResStationTest, testResStationUpdate) {

    // Testing conflict
    registerStatus.set(10, 3);
    registerStatus.set(5, 6);

    RSEntry* entry3 = resStation->add(insts[2]);

    cdb.set(3, 14);
    cdb.set(6, 1000);

    resStation->updateFromCDB();

     EXPECT_EQ(3, entry3->getQj());
     EXPECT_EQ(6, entry3->getQk());


    EXPECT_EQ(14, entry3->getVj());
    EXPECT_EQ(1000, entry3->getVk());
}

TEST_F(ResStationTest, testResStationRemove) {
    // write remove
}
