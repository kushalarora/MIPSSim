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

        void run() {
            cout << "Executor Running";
        }
};

void Executor::instFetchStage() {
    // Read memory for nextPC.
    // Add instruction to instructionQueue
    // Check BTB for next address
    // update nextPC
}


void Executor::decodeStage() {
    // Read instructionQueue. If empty do nothing
    // Else decode Instruction using instructionBuilder.
    // Check ROB and
}
#endif
