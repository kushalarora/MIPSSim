#include<queue>
#include "../instructions/Instruction.h"

using namespace std;

#ifndef __MIPS_ROB__
#define __MIPS_ROB__

class ROBSlot {
    private:
        int index;
        bool busy;
        Instruction& instruction;
        STATE state;
        unsigned int destination;
        int value;

    public:
        // TODO:: Add Constructor
        int getIndex() { return index; }
        bool isBusy() { return busy; }
        Instruction& getInstruction() { return instruction; }
        STATE getState() { return state; }
        int getDestination() { return destination; }
        int getValue() { return value; }

};


class ROB {
    private:
        queue<ROBSlot*> robQueue;
    public:
        static const int MAXSIZE = 6;
        bool isFull() { return robQueue.size() == MAXSIZE; }
        void queueInstruction(Instruction& instruction);
        void dequeueInstruction();
};

#endif
