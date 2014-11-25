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
			int min = 0;
			for (map<int, long>::iterator it =
					lastedTickedAt.begin(); it != lastedTickedAt.end(); ++it) {
				if (min == 0) {
					min = it->second;
					minKey = it->first;
				} else if (it->second < min) {
					min = it->second;
					minKey = it->first;
				}
			}

			// Erase the corresponding entry from buffer
            assert (buffer.count(minKey) > 0);
            buffer.erase(minKey);

			//Erase the entry from lastedTicketedAt
			lastedTickedAt.erase(minKey);
		} else {
			size++;
		}

		buffer[PC] = btbEntry;
		lastedTickedAt[PC] = tick;
	}
}
