#include "Simulator.h"
#include "../pipeline/BTB.h"
#include "../pipeline/ROB.h"
#include "../pipeline/ReservationStation.h"
#include "../pipeline/RegisterStatus.h"
#include "../pipeline/Registers.h"
#include "../pipeline/CDB.h"
#include <map>

#ifndef __MIPS_EXECUTOR__
#define __MIPS_EXECUTOR__
class Executor: public Simulator {
private:
	BranchTargetBuffer* btb;
	CDB* cdb;
	RegisterStatus* regStatus;
    Registers* registers;
	ROB* rob;
	ReservationStation* resStation;

	unsigned int nextPC;

	// SW address to count mapping.
	map<unsigned int, int> SWAddToCount;

	static unsigned int executionCycle;

    bool done;

    ofstream logFile;

	void flush();   // to flush all hardware on wrong branch prediction.
public:
	void instFetchStage();
	void decodeStage();
	void executeStage();
	void writeResultStage();
	void commitStage();
    string instructionQueueDump();
    string memoryDump();
    void dumpLog();

    unsigned int getNextPC() {
        return nextPC;
    }

    void setNextPC(unsigned int nextPC) {
        this->nextPC = nextPC;
    }
	static unsigned int getExecutionCycle() {
		return executionCycle;
	}

    static void setExecutionCycle(unsigned int cycle) {
        executionCycle = cycle;
    }

	Executor(char* logFileName, CDB* cdb, BranchTargetBuffer* btb, RegisterStatus* regStatus,
            Registers* registers,  ROB* rob, ReservationStation* resStation) : Simulator(logFileName) {
		nextPC = 600;

		this->btb = btb;
		this->cdb = cdb;
		this->regStatus = regStatus;
        this->registers = registers;
		this->rob = rob;
		this->resStation = resStation;

        done = false;
	}

	void run();
};
#endif
