#include "../Data.cpp"
#include "../commons.h"
#include "../instructions/Instruction.h"
#include<string>
#include<queue>
#include<vector>
#include<fstream>
#ifndef __MIPSSIM_SIMULATOR__
#define __MIPSSIM_SIMULATOR__

class Simulator {
    private:
        queue<Instruction*> instructionQueue;
        vector<Data*> memory;
        char* logFileName;
    public:
        Simulator(char* logFileName) {
            this->logFileName = logFileName;
        }
        void addInstruction(Instruction* instruction) {
            instructionQueue.push(instruction);
        }
        void addData(Data* data) {
            memory.push_back(data);
        }
        virtual void run()  = 0;
    protected:
        queue<Instruction*>& getQueue() { return instructionQueue;}
        vector<Data*> getMemory() { return memory;}
        char* getLogFileName() {return logFileName;}
};
#endif
