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
        int lineNo;
        string bitString;
    public:
        Instruction(int lineNo, string bitString) {
            this->lineNo = lineNo;
            this->bitString = bitString;
        }

        virtual string instructionString() = 0;

        INSTRUCTIONS getOpCode() { return opcode;}
        string getBitString() { return bitString;}

        string toString() {
            stringstream ss;
            ss << bitString.substr(0, 6) << " " << bitString.substr(6, 5) << " ";
            ss << bitString.substr(11, 5) << " " << bitString.substr(16, 5) << " ";
            ss << bitString.substr(21, 5) << " " << bitString.substr(26, 6);

            ss << " " << lineNo;
            ss << " " << instructionString();
            return ss.str();
        }

    protected:
        void setOpCode(INSTRUCTIONS opcode) { this->opcode = opcode;}
};

#endif
