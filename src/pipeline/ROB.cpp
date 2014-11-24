#include "ROB.h"

ROBSlot* ROB::queueInstruction(Instruction* instruction) {
    index++;
    assert (robQueue.size() < MAXSIZE);
    ROBSlot* robSlot = new ROBSlot(index, instruction);
    robQueue.push(robSlot);
    return robSlot;
}


ROBSlot* ROB::dequeueInstruction() {
    if (robQueue.size() == 0) {
        return NULL;
    }
    ROBSlot* entry = robQueue.front();
    robQueue.pop();
    return entry;
}
