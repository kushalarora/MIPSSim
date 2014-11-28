#include "../Data.cpp"
#include "../commons.h"
#include "../instructions/Instruction.h"
#include "../instructions/RawInstruction.h"
#include "../instructions/InstructionBuilder.cpp"
#include<string>
#include<queue>
#include<vector>
#include<fstream>
#ifndef __MIPSSIM_SIMULATOR__
#define __MIPSSIM_SIMULATOR__

class Simulator {
private:
	char* logFileName;

	unsigned int getIndexFromAddress(unsigned int address) {
		return (address - BASE_PC) / 4;
	}
public:
	Simulator(char* logFileName) {
		this->logFileName = logFileName;
	}
	
    void addToMemory(Data* data) {
		memory.push_back(data);
	}
    
    RawInstruction* peekInstQueue() {
        return instructionQueue.front();
    }

	Data* getMemoryData(unsigned int address) {
		unsigned int index = getIndexFromAddress(address);
		assert(index >= 0 && index < memory.size());
		return memory.at(index);
	}

	void writeToMemory(unsigned int address, Data* data) {
		int index = getIndexFromAddress(address);
		assert(index >= 0);

		if (memory.size() > index) {
			// delete the data at current memory location.
			delete memory[index];
		}
		memory[index] = data;
	}

	virtual void run() = 0;
protected:
	queue<RawInstruction*> instructionQueue;
	vector<Data*> memory;
	int registers[32];
	char* getLogFileName() {
		return logFileName;
	}
};
#endif
