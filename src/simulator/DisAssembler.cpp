#include "../commons.h"
#include "Simulator.h"
class DisAssembler : public Simulator {
    public:
        DisAssembler(char* logFileName) : Simulator(logFileName){}
        virtual void run() {
            ofstream logFile;
            logFile.open(getLogFileName());
            queue<Instruction*> q = getQueue();
            while (!q.empty()) {
                Instruction* instrP = q.front();
                logFile << instrP->toString() << endl;
                q.pop();
            }

            vector<Data*> mem = getMemory();
            vector<Data*>::iterator it;

            for (it = mem.begin(); it != mem.end(); it++) {
                logFile << (*it)->toString() << endl;
            }

            logFile.close();
        }
};
