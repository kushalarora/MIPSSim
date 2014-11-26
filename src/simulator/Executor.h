#include "Simulator.h"
#include "../pipeline/BTB.h"
#include "../pipeline/ROB.h"
#include "../pipeline/ReservationStation.h"
#include "../pipeline/RegisterStatus.h"
#include "../pipeline/CDB.h"
#include <map>

#ifndef __MIPS_EXECUTOR__
#define __MIPS_EXECUTOR__
class Executor : public Simulator {
    private:
        BranchTargetBuffer* btb;
        CDB* cdb;
        RegisterStatus* regStatus;
        ROB* rob;
        ReservationStation* resStation;

        unsigned int nextPC;

        // SW address to count mapping.
        map<unsigned int, int> SWAddToCount;

        static unsigned int executionCycle;

        void flush();   // to flush all hardware on wrong branch prediction.
    public:
        void instFetchStage();
        void decodeStage();
        void executeStage();
        void writeResultStage();
        void commitStage();

        static unsigned int getExecutionCycle() { return executionCycle; }

        Executor(char* logFileName) : Simulator(logFileName) {
            nextPC = 600;

            btb = new BranchTargetBuffer();
            cdb = new CDB();
            regStatus = new RegisterStatus();
            rob = new ROB(cdb, regStatus);
            resStation = new ReservationStation(cdb, regStatus, SWAddToCount, registers);
        }

        void run();
};
#endif
