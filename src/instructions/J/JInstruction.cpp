#include<iostream>
#include<string>
#include "../Instruction.h"
#include "../OpcodeMap.h"
using namespace std;
#ifndef __MIPS_JINST__
#define __MIPS_JINST__
class JInstruction: public Instruction {
private:
	int jumpAddress;

public:
	JInstruction(int lineNo, string bitString, unsigned int executionCycle) :
			Instruction(lineNo, bitString, executionCycle) {
		jumpAddress = toInt(bitString.substr(6, 26)) * 4;

		setOpCode(OpcodeMap::strToOpCodeMap[bitString.substr(0, 6)]);

		hasRegisterOutput = false;
	}

	int execute(int vj, int vk) {
		return jumpAddress;
	}

	bool outcome(int vj, int vk) {
		return true;
	}

	unsigned int getDestination() {
		return (getAddress() & 0xF0000000) + jumpAddress;
	}

	INSTRUCTION_TYPE getType() {
		return JTYPE;
	}

	int getArg1() {
		assert(false);
	}

	int getImmediate() {
		assert(false);
	}

	int getArg2() {
		assert(false);
	}

	string instructionString() {
		stringstream ss;
		ss << OpCodeStrings[getOpCode()] << " #" << jumpAddress;
		return ss.str();
	}
};

/*
 int main() {
 JInstruction instr = JInstruction(600, "00001000000000000000000010011110", -1);
 cout << instr.toString();
 }
 */
#endif
