#include<iostream>
#include "J/JInstruction.cpp"
#include "R/RInstruction.cpp"
#include "I/IInstruction.cpp"
#include "Instruction.h"
#include "../commons.h"
#include<string>

using namespace std;
#ifndef __MIPS_INST_BUILDER__
#define __MIPS_INST_BUILDER__
class InstructionBuilder {
public:
	static Instruction* build(int lineNo, string bitString,
			int executionCycle) {
		string opcode = bitString.substr(0, 6);
		if (opcode.compare("000000") == 0) {
			return new RInstruction(lineNo, bitString, executionCycle);
		} else if (opcode.compare("000010") == 0
				|| opcode.compare("000011") == 0) {
			return new JInstruction(lineNo, bitString, executionCycle);
		} else {
			return new IInstruction(lineNo, bitString, executionCycle);
		}
	}
	static Instruction* build(int lineNo, string bitString) {
		return build(lineNo, bitString, -1);
	}
};
#endif
