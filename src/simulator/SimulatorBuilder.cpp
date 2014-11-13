#include<iostream>
#include "../commons.h"
#include "DisAssembler.cpp"
#include "Executor.cpp"
#include "Simulator.h"

using namespace std;
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
