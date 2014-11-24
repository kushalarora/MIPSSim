#include "Simulator.h"
#include "../pipeline/BTB.h"
#include "../pipeline/ROB.h"
#include "../pipeline/ReservationStation.h"


#ifndef __MIPS_EXECUTOR__
#define __MIPS_EXECUTOR__
class Executor : public Simulator {
    private:
        BranchTargetBuffer btb;
        ROB rob;
        ReservationStation resStation;
        unsigned int nextPC;
        unsigned int registerStatus[32];
        unsigned int executionCycle;
    public:
        void instFetchStage();
        void decodeStage();
        void executeStage();
        void writeResultStage();
        void commitStage();
        Executor(char* logFileName) : Simulator(logFileName) {
            nextPC = 600;
            executionCycle = 0;
        }

        void run();
};
#endif
