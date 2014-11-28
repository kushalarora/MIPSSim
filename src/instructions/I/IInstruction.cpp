#include<iostream>
#include<string>
#include "../Instruction.h"
#include "../OpcodeMap.h"

using namespace std;
#ifndef __MIPS_IINST__
#define __MIPS_IINST__
class IInstruction: public Instruction {
private:
	int registerS;
	int registerT;
	int immediate;

	bool outcomeBEQ(int vj, int vk) {
		return (vj == vk);
	}

	bool outcomeBNE(int vj, int vk) {
		return (vj != vk);
	}

	bool outcomeBGEZ(int vj, int vk) {
		return (vj >= 0);
	}

	bool outcomeBGTZ(int vj, int vk) {
		return (vj > 0);
	}

	bool outcomeBLEZ(int vj, int vk) {
		return (vj <= 0);
	}

	bool outcomeBLTZ(int vj, int vk) {
		return (vj < 0);
	}

	int executeADDI(int vj, int vk) {
		return immediate + vj;
	}

	int executeADDIU(int vj, int vk) {
		return immediate + vj;
	}

	int executeSLTI(int vj, int vk) {
		return (vj < immediate);
	}

public:
	IInstruction(int lineNo, string bitString, unsigned int executionCycle) :
			Instruction(lineNo, bitString, executionCycle) {
		registerS = toInt(bitString.substr(6, 5));
		registerT = toInt(bitString.substr(11, 5));
		immediate = fromTwosComplement(toInt(bitString.substr(16, 16)), 16);

		setOpCode(OpcodeMap::strToOpCodeMap[bitString.substr(0, 6)]);

		INSTRUCTIONS opcode = getOpCode();
		if (opcode == BEQ || opcode == BNE || opcode == BGTZ || opcode == BLEZ
				|| opcode == BLTZ || opcode == BGEZ) {
			isBranch = true;
		}

		if (opcode == SW || opcode == LW) {
			executeCyclesLeft = 2;
		}

	}

	int execute(int vj, int vk) {
		switch (getOpCode()) {
		case ADDI:
			return executeADDI(vj, vk);
			break;
		case ADDIU:
			return executeADDIU(vj, vk);
			break;
		case SLTI:
			return executeSLTI(vj, vk);
			break;
		}
	}

	bool outcome(int vj, int vk) {
		switch (getOpCode()) {
		case BEQ:
			return outcomeBEQ(vj, vk);
			break;
		case BNE:
			return outcomeBNE(vj, vk);
			break;
		case BGTZ:
			return outcomeBGTZ(vj, vk);
			break;
		case BLEZ:
			return outcomeBLEZ(vj, vk);
			break;
		case BLTZ:
			return outcomeBLTZ(vj, vk);
			break;
		case BGEZ:
			return outcomeBGEZ(vj, vk);
			break;
		}
	}

	unsigned int getDestination() {
		return (isBranch ? getAddress() + immediate * 4 : registerT);
	}

	INSTRUCTION_TYPE getType() {
		return ITYPE;
	}

	int getArg1() {
		return registerS;
	}

	int getImmediate() {
		return immediate;
	}

	int getArg2() {
		assert(false);
	}

	string instructionString() {
		stringstream ss;
		ss << OpCodeStrings[getOpCode()] << " ";
		switch (getOpCode()) {
		case LW:
		case SW:
			ss << "R" << registerT << ", " << immediate << "(" << "R"
					<< registerS << ")";
			break;
		case BEQ:
		case BNE:
			ss << "R" << registerS << ", " << "R" << registerT << ", #"
					<< immediate * 4;
			break;
		case BGEZ:
		case BGTZ:
		case BLEZ:
		case BLTZ:
			ss << "R" << registerS << ", #" << immediate * 4;
			break;
		case ADDI:
		case ADDIU:
		case SLTI:
			ss << "R" << registerT << ", " << "R" << registerS << ", #"
					<< immediate;
			break;
		default:
			cerr << "Not a Immediate type opcode";

		};
		return ss.str();
	}
};

/*
 int main() {
 IInstruction instr1 = IInstruction(600, "00100000000010000000000000101100", -1);
 cout << instr1.toString();
 cout << "\n";
 IInstruction instr2 = IInstruction(604, "10001100100000100000001011001100", -1);
 cout << instr2.toString();
 cout << "\n";
 IInstruction instr3 = IInstruction(608, "00010001010010000000000000000001", -1);
 cout << instr3.toString();
 cout << "\n";
 }*/
#endif
