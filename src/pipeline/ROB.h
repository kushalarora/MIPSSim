#include<queue>
#include "../instructions/Instruction.h"
#include<cassert>
#include<climits>
using namespace std;

#ifndef __MIPS_ROB__
#define __MIPS_ROB__

class ROBSlot {
    private:
        unsigned int index;
        bool ready;
        Instruction* instruction;
        unsigned int destination;
        int value;

    public:
        // TODO:: Add Constructor
        int getIndex() { return index; }
        bool isReady() { return ready; }
        Instruction* getInstruction() { return instruction; }
        int getDestination() { return destination; }
        int getValue() { return value; }
        void setValue() { this->value = value;}

        ROBSlot(int index, Instruction* instruction) {
           ready = false;
           this->instruction  = instruction;

           // determine destination
           // TODO::Incomplete
           if (instruction->getOpCode() == SW) {
                destination = -1;
           }
           value = INT_MIN;
        }
};


class ROB {
    private:
        queue<ROBSlot*> robQueue;
        int index;  // index starts at 1.
    public:
        static const int MAXSIZE = 6;
        bool isFull() { return robQueue.size() == MAXSIZE; }
        ROBSlot* queueInstruction(Instruction* instruction);
        ROBSlot* dequeueInstruction();

        ROB() {
            index = 0;
        }
};

#endif
