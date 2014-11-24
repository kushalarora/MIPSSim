#include<iostream>
#include<string>
#include "../Instruction.h"
#include "../OpcodeMap.h"
using namespace std;
#ifndef __MIPS_JINST__
#define __MIPS_JINST__
class JInstruction : public Instruction  {
    private:
        int address;
    public:
        JInstruction(int lineNo, string bitString, unsigned int executionCycle) : Instruction(lineNo, bitString, executionCycle) {
            address = toInt(bitString.substr(6, 26)) * 4;

            setOpCode(OpcodeMap::strToOpCodeMap[bitString.substr(0, 6)]);
        }

        string instructionString() {
            stringstream ss;
            ss << OpCodeStrings[getOpCode()] << " #" << address;
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
