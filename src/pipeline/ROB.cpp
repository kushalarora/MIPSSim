#include "ROB.h"
#include "../simulator/Executor.h"

const int ROBSlot::DEFAULT_VALUE = INT_MIN;
ROBSlot* ROB::queueInstruction(Instruction* instruction) {
	// Verify the slot is there
	assert(robQueue.size() < MAXSIZE);

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
	this->instruction = instruction;
	destination = instruction->getDestination();
	value = DEFAULT_VALUE;
}

void ROB::updateFromCDB() {
	for (deque<ROBSlot*>::iterator it = robQueue.begin(); 
            it != robQueue.end(); it++) {

		ROBSlot* slot = *it;
		unsigned int ROBId = slot->getIndex();

		Instruction* inst = slot->getInstruction();
		int executionCycle = Executor::getExecutionCycle();

		// If scheduled for future cycles, ignore.
		if (inst->getExecutionCycle() > executionCycle) {
			continue;
		}

        // if already ready, just continue
		if (slot->isReady()) {
			continue;
		}

		// If ROBId in cdb.
		// Update value and make it ready for commit
		// Set the instruction execution cycle to next
		// cycle to ensure commit in next cycle or later.
		int value = cdb->get(ROBId);
		if (value != CDB::DEFAULT_VALUE) {
			slot->setValue(value);
			inst->setExecutionCycle(executionCycle + 1);
			slot->makeReady();
		}
	}
}

string ROB::robDump() {
    stringstream ss;
    ss << "ROB:";
	for (deque<ROBSlot*>::iterator it = robQueue.begin();
            it != robQueue.end(); it++) {
		ROBSlot* slot = *it;
		Instruction* inst = slot->getInstruction();
        ss << endl << "[" << inst->instructionString() << "] ";
	}
    return ss.str();
}


int ROB::getValue(int ROBId) {
    int value = ROBSlot::DEFAULT_VALUE;
    for (deque<ROBSlot*>::iterator it = robQueue.begin();
            it != robQueue.end(); it++) {
        ROBSlot* slot = *it;
        if (slot->isReady() && slot->getIndex() == ROBId) {
            value = slot->getValue();
            break;
        }
    }

    return value;
}

