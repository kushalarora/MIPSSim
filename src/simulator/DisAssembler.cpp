#include "../commons.h"
#include "Simulator.h"
class DisAssembler : public Simulator {
    public:
        DisAssembler(char* logFileName) : Simulator(logFileName){}
        virtual void run() {
            ofstream logFile;
            logFile.open(getLogFileName());

            vector<Data*> mem = getMemory();
            vector<Data*>::iterator it;

            bool isBreak = false;
            for (it = mem.begin(); it != mem.end(); it++) {
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
