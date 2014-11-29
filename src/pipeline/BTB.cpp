#include "BTB.h"

#include "../commons.h"

const int BTBEntry::NOT_SET = -1;
const int BTBEntry::NOT_TAKEN=  0;
const int BTBEntry::TAKEN = 1;
unsigned int BranchTargetBuffer::getNextPC(unsigned int PC, unsigned int nextPC) {
	assert(PC >= BASE_PC);

	tick++; // Update the tick to keep track of current cycle

	if (buffer.count(PC) > 0) {
		BTBEntry* btbEntry = buffer.at(PC);

		if (btbEntry->outcome == BTBEntry::TAKEN) {
			nextPC = btbEntry->predictedPC;
            cout << "Predicted next PC: " <<  nextPC << endl;
		} else {
			// if not taken last time, increment 4
			nextPC = PC + 4;
		}

		// Update the tick value to keep LRU behavior
		assert(lastedTickedAt.count(PC) > 0);
		lastedTickedAt[PC] = tick;

	} else {

		BTBEntry* btbEntry = new BTBEntry(nextPC, BTBEntry::NOT_SET);
        cout << "BTB: " << PC << "Outcome: " << btbEntry->outcome << " NextPC: " << btbEntry->predictedPC << endl;
		// If BTB is full then delete the LRU entry
		if (size == MAXSIZE) {
			int minKey = 0;
			int min = 0;
			for (map<int, long>::iterator it = lastedTickedAt.begin();
					it != lastedTickedAt.end(); ++it) {
				if (min == 0) {
					min = it->second;
					minKey = it->first;
				} else if (it->second < min) {
					min = it->second;
					minKey = it->first;
				}
			}

			// Erase the corresponding entry from buffer
			assert(buffer.count(minKey) > 0);
			buffer.erase(minKey);

			//Erase the entry from lastedTicketedAt
			lastedTickedAt.erase(minKey);
		} else {
			size++;
		}

		buffer[PC] = btbEntry;
		lastedTickedAt[PC] = tick;
		nextPC = PC + 4;
    }
	return nextPC;
}

void BranchTargetBuffer::updateOrAdd(unsigned int PC,  int outcome) {
	assert(PC >= BASE_PC);
    assert(buffer.count(PC) > 0);
    assert(lastedTickedAt.count(PC) > 0);


    BTBEntry* btbEntry = buffer.at(PC);
    btbEntry->outcome = outcome;
}


string BranchTargetBuffer::btbDump() {
    stringstream ss;
    ss << "BTB:";
    int i = 1;
    for (map<int, BTBEntry*>::iterator it = buffer.begin();
            it != buffer.end(); it++) {
        ss << endl << "[" << "Entry " << i++ << "]:";

        BTBEntry* entry = it->second;
        // TODO::What is not set funda
        ss << "<" << it->first << "," << entry->predictedPC<<",";
        if (entry->outcome == BTBEntry::NOT_SET)  {
        	ss <<"NotSet";
        } else {
        	ss << entry->outcome;
        }
        ss << ">";

    }
    return ss.str();
}
