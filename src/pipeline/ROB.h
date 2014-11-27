#include<deque>
#include "../commons.h"
#include<cassert>
#include<climits>
#include "../instructions/Instruction.h"
#include "CDB.h"
#include "RegisterStatus.h"
using namespace std;

#ifndef __MIPS_ROB__
#define __MIPS_ROB__

class ROBSlot {
    private:
        unsigned int index;
        bool ready;
        Instruction* instruction;
        unsigned int destination;
        bool branchOutcome;
        int value;

    public:
        int getIndex() { return index; }

        bool isReady() { return ready; }
        bool makeReady() { ready = true; }

        Instruction* getInstruction() { return instruction; }

        int getDestination() { return destination; }
        void setDestination(unsigned int destination){this->destination = destination;}

        bool getBranchOutcome(){return branchOutcome;}
        void setBranchOutcome(bool outcome){this->branchOutcome = outcome;}

        int getValue() { return value; }
        void setValue(int value) { this->value = value;}

        ROBSlot(int index, Instruction* instruction);
};


class ROB {
    private:
        deque<ROBSlot*> robQueue;
        int index;  // index starts at 1.
        CDB* cdb;
        RegisterStatus* registerStatus;

    public:
        static const int MAXSIZE = 6;

        bool isFull() { return robQueue.size() == MAXSIZE; }

        ROBSlot* queueInstruction(Instruction* instruction);

        ROBSlot* dequeueInstruction();

        ROBSlot* peekTop() { return robQueue.front(); }

        void flush() { robQueue.clear(); }

        ROB(CDB* cdb, RegisterStatus* registerStatus)
            : cdb(cdb), registerStatus(registerStatus) {
            index = 0;
        }

        void updateFromCDB();
};
#endif
