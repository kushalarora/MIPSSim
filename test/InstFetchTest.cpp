#include "../src/simulator/Executor.h"
#include<vector>
#include "gtest/gtest.h"
#include "../src/pipeline/BTB.h"
#include "../src/pipeline/ROB.h"
#include "../src/pipeline/ReservationStation.h"
#include "../src/pipeline/RegisterStatus.h"
#include "../src/pipeline/CDB.h"
#include <map>



class InstFetchTest : public ::testing::Test {
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


	InstFetchTest() {
        cdb =  new CDB();
        btb = new BranchTargetBuffer();
        regStatus = new RegisterStatus();
        rob = new ROB(cdb, regStatus);
        registers = new Registers();
        resStation = new ReservationStation(cdb, regStatus, tempMap, registers);
		 data.push_back(new Data(600, "00100000000010000000000000101100"));
		 data.push_back(new Data(604, "10001100100000100000001011001100"));
         char* fileName = "/home/karora/dummyName.txt";
         executor = new Executor(fileName, cdb, btb, regStatus, registers, rob, resStation);
         executor->addToMemory(data[0]);
         executor->addToMemory(data[1]);
	}
};


TEST_F(InstFetchTest, testFetch) {

    executor->instFetchStage();

    EXPECT_EQ(604, executor->getNextPC());
    RawInstruction* inst = executor->peekInstQueue();
    EXPECT_EQ(Executor::getExecutionCycle() + 1, inst->getDecodeCycle());
    EXPECT_EQ(600, inst->getAddress());
}

TEST_F(InstFetchTest, testBTBNextPC) {
    btb->updateOrAdd(600, 700, false);
    executor->instFetchStage();
    EXPECT_EQ(604, executor->getNextPC());

    btb->updateOrAdd(604, 704, true);
    executor->instFetchStage();
    EXPECT_EQ(704, executor->getNextPC());
}

