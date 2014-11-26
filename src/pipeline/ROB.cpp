#include "ROB.h"
#include "../simulator/Executor.h"

ROBSlot* ROB::queueInstruction(Instruction* instruction) {
    // Verify the slot is there
    assert (robQueue.size() < MAXSIZE);

    // Create the entry in ROB
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

ROBSlot::ROBSlot(int index, Instruction* instruction) {
    this->index = index;
    ready = false;
    this->instruction  = instruction;
    INSTRUCTIONS opCode = instruction->getOpCode();
    destination = instruction->getDestination();
    value = INT_MIN;
}

void ROB::updateFromCDB() {
   for (deque<ROBSlot*>::iterator it = robQueue.begin();
           it != robQueue.end(); it++) {

       ROBSlot* slot = *it;
       unsigned int ROBId = slot->getIndex();

       // If ROBId in cdb.
       // Update value and make it ready for commit
       // Set the instruction execution cycle to next
       // cycle to ensure commit in next cycle or later.
       int value = cdb->get(ROBId);
       if (value != CDB::DEFAULT_VALUE) {
            slot->setValue(value);
            slot->makeReady();
            (slot->getInstruction())->setExecutionCycle(
                    Executor::getExecutionCycle() + 1);
       }
   }
}
