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
    public:
        void instFetchStage();
        void decodeStage();
        void executeStage();
        void writeResultStage();
        void commitStage();
        Executor(char* logFileName) : Simulator(logFileName) {
            nextPC = 600;
        }

        void run();
};
#endif
