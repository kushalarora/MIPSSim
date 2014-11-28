#include<iostream>
#include<string>
#include "../commons.h"
#include<sstream>
#include<map>

#ifndef __MIPSSIM_INSTRUCTION__
#define __MIPSSIM_INSTRUCTION__

using namespace std;

class ROBSlot;
class Instruction {
private:
	INSTRUCTIONS opcode;
	int address;
	string bitString;
	ROBSlot* robSlot;
	unsigned int RSId;
public:
	Instruction(int address, string bitString) {
		this->address = address;
		this->bitString = bitString;
		executionCycle = -1;
		isBranch = false;
		robSlot = NULL;
		RSId = -1;
		hasRegisterOutput = true;
		executeCyclesLeft = 1;
	}

	Instruction(unsigned int address, string bitString,
			unsigned int executionCycle) {
		this->address = address;
		this->bitString = bitString;
		this->executionCycle = executionCycle;
		isBranch = false;
		robSlot = NULL;
		RSId = -1;
		hasRegisterOutput = true;
		executeCyclesLeft = 1;
	}

	virtual string instructionString() = 0;

	INSTRUCTIONS getOpCode() {
		return opcode;
	}

	string getBitString() {
		return bitString;
	}

	unsigned int getAddress() {
		return address;
	}

	ROBSlot* getROBSlot() {
		return robSlot;
	}
	void setROBSlot(ROBSlot* robSlot) {
		this->robSlot = robSlot;
	}

	int getRSId() {
		return RSId;
	}
	void setRSId(int RSId) {
		this->RSId = RSId;
	}

	unsigned int getExecutionCycle() {
		return executionCycle;
	}
	void setExecutionCycle(unsigned int executionCycle) {
		this->executionCycle = executionCycle;
	}

	bool isBranchInst() {
		return isBranch;
	}

	bool writesToRegister() {
		return hasRegisterOutput;
	}

	virtual int getArg1() = 0;

	virtual int getArg2() = 0;

	virtual int getImmediate() = 0;

	virtual INSTRUCTION_TYPE getType() = 0;

	virtual unsigned int getDestination() = 0;

	string toString() {
		stringstream ss;
		ss << bitString.substr(0, 6) << " " << bitString.substr(6, 5) << " ";
		ss << bitString.substr(11, 5) << " " << bitString.substr(16, 5) << " ";
		ss << bitString.substr(21, 5) << " " << bitString.substr(26, 6);

		ss << " " << address;
		ss << " " << instructionString();
		return ss.str();
	}

protected:
	void setOpCode(INSTRUCTIONS opcode) {
		this->opcode = opcode;
	}
	bool isBranch;
	bool hasRegisterOutput;
	unsigned int executionCycle;
	unsigned int executeCyclesLeft;

};

#endif
