#include<iostream>
#include "../commons.h"
#include "DisAssembler.cpp"
#include "Executor.h"
#include "Simulator.h"

using namespace std;

#ifndef __MIPS_SIMBUILDER__
#define __MIPS_SIMBUILDER__
class SimulatorBuilder {
    public:
        static Simulator* build(char* outFile, Operation operation) {
            if (operation == DIS) {
                return new DisAssembler(outFile);
            } else if (operation == SIM) {
                return new Executor(outFile);
            }
            cerr << "Unknown Operation: " << operation;
        }
};
#endif
