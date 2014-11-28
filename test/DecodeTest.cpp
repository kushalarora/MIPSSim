
#include "../src/simulator/Executor.h"
#include<vector>
#include "gtest/gtest.h"
#include "../src/pipeline/BTB.h"
#include "../src/pipeline/ROB.h"
#include "../src/pipeline/ReservationStation.h"
#include "../src/pipeline/RegisterStatus.h"
#include "../src/pipeline/CDB.h"
#include <map>

class DecodeTest : public ::testing::Test {
public:
	vector<Data*> data;
    Executor* executor;
    CDB* cdb;
    BranchTargetBuffer* btb;
    RegisterStatus* regStatus;
    Registers* registers;
    ROB* rob;
    map<unsigned int, int> tempMap;
    ReservationStation* resStation;


	DecodeTest() {
        cdb =  new CDB();
        btb = new BranchTargetBuffer();
        regStatus = new RegisterStatus();
        rob = new ROB(cdb, regStatus);
        registers = new Registers();
        resStation = new ReservationStation(cdb, regStatus, tempMap, registers);
        // ADDI R8, R0, #44
		 data.push_back(new Data(600, "00100000000010000000000000101100"));
        // LW R2, 716(R4)
		 data.push_back(new Data(604, "10001100100000100000001011001100"));
        // SW R5, 716(R10)
         data.push_back(new Data(608, "10101101010001010000001011001100"));

         char* fileName = "/home/karora/dummyName.txt";
         executor = new Executor(fileName, cdb, btb, regStatus, registers, rob, resStation);
         executor->addToMemory(data[0]);
         executor->addToMemory(data[1]);
	}
};

TEST_F(DecodeTest, testEmptyInstQueue) {
    executor->decodeStage();

    EXPECT_TRUE(rob->isEmpty());
}

TEST_F(DecodeTest, testInstCycle) {
    executor->instFetchStage();

    executor->decodeStage();

    EXPECT_TRUE(rob->isEmpty());
}

TEST_F(DecodeTest, testNOP) {
    executor->addToMemory(new Data(608, "00000000000000000000000000000000"));
    executor->setNextPC(608);
    executor->instFetchStage();
    Executor::setExecutionCycle(Executor::getExecutionCycle() + 4);
    executor->decodeStage();

    EXPECT_FALSE(rob->isEmpty());
    EXPECT_EQ(0, resStation->size());

    ROBSlot* slot = rob->peekTop();
    EXPECT_EQ(NOP, slot->getInstruction()->getOpCode());
}

TEST_F(DecodeTest, testBREAK) {
    executor->addToMemory(new Data(608, "00000000000000000000000000001101"));
    executor->setNextPC(608);
    executor->instFetchStage();
    Executor::setExecutionCycle(Executor::getExecutionCycle() + 4);
    executor->decodeStage();

    EXPECT_FALSE(rob->isEmpty());
    EXPECT_EQ(0, resStation->size());

    ROBSlot* slot = rob->peekTop();
    EXPECT_EQ(BREAK, slot->getInstruction()->getOpCode());
}


TEST_F(DecodeTest, testALUInst) {
    executor->instFetchStage();
    Executor::setExecutionCycle(Executor::getExecutionCycle() + 1);
    executor->decodeStage();

    EXPECT_FALSE(rob->isEmpty());
    EXPECT_EQ(1, resStation->size());

    ROBSlot* slot = rob->peekTop();
    Instruction* inst = slot->getInstruction();
    EXPECT_EQ(slot, inst->getROBSlot());
    EXPECT_TRUE(inst->writesToRegister());
    int destination = inst->getDestination();
    EXPECT_TRUE(regStatus->isSet(destination));
    EXPECT_EQ(slot->getIndex(), regStatus->get(destination));

}
