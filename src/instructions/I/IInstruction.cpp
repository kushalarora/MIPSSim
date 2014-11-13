#include<iostream>
#include<string>
#include "../Instruction.h"
#include "../OpcodeMap.h"

using namespace std;

class IInstruction : public Instruction {
    private:
        int registerS;
        int registerT;
        int immediate;
    public:
        IInstruction(int lineNo, string bitString) : Instruction(lineNo, bitString) {
            registerS = toInt(bitString.substr(6, 5));
            registerT = toInt(bitString.substr(11, 5));
            immediate = toInt(bitString.substr(16, 16));

            setOpCode(OpcodeMap::strToOpCodeMap[bitString.substr(0, 6)]);
        }
        string instructionString() {
            stringstream ss;
            ss << OpCodeStrings[getOpCode()] << " ";
            switch (getOpCode()) {
                case LW :
                case SW :
                    ss << "R" << registerT << ", " << immediate << "(" << "R" << registerS << ")";
                    break;
                case BEQ:
                case BNE:
                    ss << "R" << registerS << ", " << "R" << registerT << ", #" << immediate * 4;
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
                    ss << "R" << registerT << ", " << "R" << registerS << ", #" << immediate;
                    break;
                default:
                    cerr << "Not a Immediate type opcode";

            };
            return ss.str();
        }
};

/*
int main() {
    IInstruction instr1 = IInstruction(600, "00100000000010000000000000101100");
    cout << instr1.toString();
    cout << "\n";
    IInstruction instr2 = IInstruction(604, "10001100100000100000001011001100");
    cout << instr2.toString();
    cout << "\n";
    IInstruction instr3 = IInstruction(608, "00010001010010000000000000000001");
    cout << instr3.toString();
    cout << "\n";
}*/