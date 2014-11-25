#include "Simulator.h"
#include "../pipeline/BTB.h"
#include "../pipeline/ROB.h"
#include "../pipeline/ReservationStation.h"
#include "../pipeline/RegisterStatus.h"
#include "../pipeline/CDB.h"


#ifndef __MIPS_EXECUTOR__
#define __MIPS_EXECUTOR__
class Executor : public Simulator {
    private:
        BranchTargetBuffer btb;
        ROB rob;
        CDB cdb;
        ReservationStation resStation;
        unsigned int nextPC;
        RegisterStatus regStatus;
        unsigned int executionCycle;
        void flush();
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
