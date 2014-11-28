#include "../../src/pipeline/ROB.h"
#include "../../src/simulator/Executor.h"
#include "../../src/instructions/InstructionBuilder.cpp"
#include<vector>
#include "gtest/gtest.h"



class ROBTest : public ::testing::Test {
public:
	vector<Instruction*> insts;
    CDB cdb;
    RegisterStatus registerStatus;
    ROB* rob;

	ROBTest() {
		 insts.push_back(InstructionBuilder::build(600, "00100000000010000000000000101100", 1));
		 insts.push_back(InstructionBuilder::build(604, "10001100100000100000001011001100", 1));
         rob = new ROB(&cdb, &registerStatus);
	}
};

TEST_F(ROBTest, testROBFull) {
    for (int i = 0; i < 5; i++) {
        rob->queueInstruction(insts[0]);
    }

    EXPECT_FALSE(rob->isFull());

    rob->queueInstruction(insts[1]);
    EXPECT_TRUE(rob->isFull());

}

TEST_F(ROBTest, testROBEmpty) {
    EXPECT_TRUE(rob->isEmpty());
    rob->queueInstruction(insts[0]);

    EXPECT_FALSE(rob->isEmpty());
}

TEST_F(ROBTest, testROBDequeue) {
    ROBSlot* slot = rob->queueInstruction(insts[0]);
    ROBSlot* slotOut = rob->dequeueInstruction();

    EXPECT_EQ(slot, slotOut);
    EXPECT_TRUE(rob->isEmpty());
}

TEST_F(ROBTest, testEntry) {
    ROBSlot* slot = rob->queueInstruction(insts[0]);
    EXPECT_EQ(1, slot->getIndex());
    EXPECT_FALSE(slot->isReady());
    EXPECT_EQ(slot->getInstruction(), insts[0]);
    EXPECT_EQ(INT_MIN, slot->getValue());
}

TEST_F(ROBTest, testUpdate) {
    ROBSlot* slot = rob->queueInstruction(insts[0]);
    ROBSlot* slot2 = rob->queueInstruction(insts[1]);
    EXPECT_EQ(INT_MIN, slot->getValue());
    EXPECT_EQ(INT_MIN, slot2->getValue());

    Executor::setExecutionCycle(1);
    cdb.set(slot->getIndex(), 10);

    rob->updateFromCDB();

    ROBSlot* slotOut = rob->peekTop();
    EXPECT_EQ(slot->getIndex(), slotOut->getIndex());

    cout <<slot->getInstruction()->getExecutionCycle()<<endl;
    EXPECT_EQ(slotOut->getValue(), 10);
    EXPECT_TRUE(slotOut->isReady());

    rob->dequeueInstruction();
    ROBSlot* slotOut2 = rob->peekTop();
    EXPECT_FALSE(slotOut2->isReady());
}
