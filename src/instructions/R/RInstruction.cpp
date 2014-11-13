#include<iostream>
#include<string>
#include "../Instruction.h"
#include "../../commons.h"
#include "../OpcodeMap.h"
#include<map>

using namespace std;

class RInstruction : public Instruction {
    private:
        int registerS;
        int registerT;
        int registerD;
        int shiftAmount;
    public:
        RInstruction(int lineNo, string bitString) : Instruction(lineNo, bitString) {
            registerS = toInt(bitString.substr(6, 5));
            registerT = toInt(bitString.substr(11, 5));
            registerD = toInt(bitString.substr(16, 5));
            shiftAmount = toInt(bitString.substr(21, 5));

            if (toInt(getBitString()) == 0) {
                setOpCode(NOP);
            } else {
                setOpCode(OpcodeMap::functionToOpCodeMap[bitString.substr(26, 32)]);
            }

        }

        virtual string instructionString() {
            if (getOpCode() == NOP || getOpCode() == BREAK) {
                return OpCodeStrings[getOpCode()];
            }

            stringstream ss;
            ss << OpCodeStrings[getOpCode()] << " " \
                << "R" << registerD << ", " \
                << "R" << registerS << ", ";

            if (getOpCode() == SLL ||
                    getOpCode() == SRL ||
                    getOpCode() == SRA) {
                    ss << "#" << shiftAmount;
            } else {
                    ss << "R" << registerT;
            }
            return ss.str();
        }
};

/*
int main() {
    RInstruction instr1 = RInstruction(600, "00000000000000000000000000000000");
    cout << instr1.toString();
    cout << "\n";
    RInstruction instr2 = RInstruction(604, "00000000000000000000000000001101");
    cout << instr2.toString();
    cout << "\n";
    RInstruction instr3 = RInstruction(608, "00000000001000100010100000100000");
    cout << instr3.toString();
    cout << "\n";
}
*/
