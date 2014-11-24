#include "BTB.h"

#include "../commons.h"

unsigned int BranchTargetBuffer::getNextPC(unsigned int PC) {
	assert(PC >= BASE_PC);
	unsigned int nextPC = PC + 4;

	tick++; // Update the tick to keep track of current cycle	

	if (buffer.count(PC) > 0) {
		BTBEntry* btbEntry = buffer.at(PC);

		if (btbEntry->prediction) {
			nextPC = btbEntry->predictedPC;

		}

		// Update the tick value to keep LRU behavior
		assert(lastedTickedAt.count(PC) > 0);
		lastedTickedAt[PC] = tick;
	}
	return nextPC;
}

void BranchTargetBuffer::updateOrAdd(unsigned int PC, unsigned int nextPC,
		bool taken) {
	assert(PC >= BASE_PC);
	assert(nextPC >= BASE_PC);

	if (buffer.count(PC) > 0) {
		BTBEntry* btbEntry = buffer.at(PC);
		btbEntry->predictedPC = nextPC;
		btbEntry->prediction = taken;
	} else {
		BTBEntry* btbEntry = new BTBEntry(nextPC, taken);

		// If BTB is full then delete the LRU entry
		if (size == MAXSIZE) {
			int minKey = 0;
			map<int, long long>::const_iterator minIt;
			int min = 0;
			for (map<int, long long>::const_iterator it =
					lastedTickedAt.cbegin(); it != lastedTickedAt.end(); ++it) {
				if (min == 0) {
					min = it->second;
					minKey = it->first;
					minIt = it;
				} else if (it->second < min) {
					min = it->second;
					minKey = it->first;
					minIt = it;
				}
			}

			// Erase the corresponding entry from buffer
			for (map<int, BTBEntry*>::const_iterator it = buffer.cbegin();
					it != buffer.cend(); ++it) {
				if (it->first == minKey) {
					buffer.erase(it);
					break;
				}
			}

			//Erase the entry from lastedTicketedAt
			lastedTickedAt.erase(minIt);
		} else {
			size++;
		}

		buffer[PC] = btbEntry;
		lastedTickedAt[PC] = tick;
	}

}
