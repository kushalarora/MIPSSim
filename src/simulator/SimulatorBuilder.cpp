#include<iostream>
#include "../commons.h"
#include "DisAssembler.cpp"
#include "Executor.h"
#include "Simulator.h"
#include "../pipeline/BTB.h"
#include "../pipeline/ROB.h"
#include "../pipeline/ReservationStation.h"
#include "../pipeline/RegisterStatus.h"
#include "../pipeline/Registers.h"
#include "../pipeline/CDB.h"
#include <map>

using namespace std;

#ifndef __MIPS_SIMBUILDER__
#define __MIPS_SIMBUILDER__
class SimulatorBuilder {
public:
	static Simulator* build(char* outFile, Operation operation) {
		if (operation == DIS) {
			return new DisAssembler(outFile);
		} else if (operation == SIM) {

            CDB* cdb = new CDB();
            BranchTargetBuffer* btb = new BranchTargetBuffer();
            RegisterStatus* regStatus = new RegisterStatus();
            ROB* rob = new ROB(cdb, regStatus);
            map<unsigned int, int> tempMap;
            Registers* registers = new Registers();
            ReservationStation* resStation = new ReservationStation(cdb, regStatus, tempMap, registers, rob);

            return new Executor(outFile, cdb, btb, regStatus, registers, rob, resStation);
		}
		cerr << "Unknown Operation: " << operation;
	}
};
#endif
