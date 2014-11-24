#include "../commons.h"
#include "Simulator.h"

#ifndef __MIPS_DISASSEMBLER__
#define __MIPS_DISASSEMBLER__
class DisAssembler : public Simulator {
    public:
        DisAssembler(char* logFileName) : Simulator(logFileName){}
        virtual void run() {
            ofstream logFile;
            logFile.open(getLogFileName());

            vector<Data*>::iterator it;

            bool isBreak = false;
            for (it = memory.begin(); it != memory.end(); it++) {
                Data* data = *it;
                if (!isBreak) {
                    Instruction* instP = InstructionBuilder::build(data->getAddress(), data->getBitString());
                    logFile << instP->toString() << endl;
                    isBreak = instP->getOpCode() == BREAK;
                } else {
                    logFile << (*it)->toString() << endl;
                }
            }

            logFile.close();
        }
};
#endif
