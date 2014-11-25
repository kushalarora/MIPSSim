#include "ROB.h"

ROBSlot* ROB::queueInstruction(Instruction* instruction) {
    assert (robQueue.size() < MAXSIZE);
    ROBSlot* robSlot = new ROBSlot(++index, instruction);
    robQueue.push_back(robSlot);
    return robSlot;
}


ROBSlot* ROB::dequeueInstruction() {
    if (robQueue.size() == 0) {
        return NULL;
    }
    ROBSlot* entry = robQueue.front();
    robQueue.pop_front();
    return entry;
}
