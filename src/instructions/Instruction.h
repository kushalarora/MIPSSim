#include<iostream>
#include<string>
#include "../commons.h"
#include<sstream>
#include<map>


#ifndef __MIPSSIM_INSTRUCTION__
#define __MIPSSIM_INSTRUCTION__
using namespace std;
class Instruction {
    private:
        INSTRUCTIONS opcode;
        int address;
        string bitString;
        unsigned int ROBId;
        unsigned int executionCycle;
    public:
        Instruction(int address, string bitString) {
            this->address = address;
            this->bitString = bitString;
            executionCycle = -1;
        }

        Instruction(unsigned int address, string bitString, unsigned int executionCycle) {
            this->address = address;
            this->bitString = bitString;
            this->executionCycle = executionCycle;
        }

        virtual string instructionString() = 0;

        INSTRUCTIONS getOpCode() { return opcode;}
        string getBitString() { return bitString;}

        unsigned int getROBId() { return ROBId; }
        void setROBId(unsigned int ROBId) { this->ROBId = ROBId; }
        unsigned int getExecutionCycle() { return executionCycle; }

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
        void setOpCode(INSTRUCTIONS opcode) { this->opcode = opcode;}

};

#endif
