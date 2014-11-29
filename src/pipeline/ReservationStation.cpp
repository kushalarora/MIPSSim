#include "ReservationStation.h"
#include "../simulator/Executor.h"
const int RSEntry::DEFAULT_VALUE = INT_MIN;
const int RSEntry::DEFAULT_Q = 0;

RSEntry::RSEntry(int RSId, Instruction* instruction,
        int Vj, int Vk, int Qj, int Qk, unsigned int A) {
	this->instruction = instruction;
	this->RSId = RSId;
	this->Vj = Vj;
	this->Vk = Vk;
	this->Qj = Qj;
	this->Qk = Qk;
	this->A = A;
}

RSEntry* ReservationStation::add(Instruction* instruction) {
	assert(reservations.size() < MAX_SIZE);

	int Vj = RSEntry::DEFAULT_VALUE, Vk = RSEntry::DEFAULT_VALUE;
	int Qj = RSEntry::DEFAULT_Q, Qk = RSEntry::DEFAULT_Q;
	int A = 0;
	// Code to check registerstatus and update Vj, Vk, Qj, Qk.
	// Address field to be populated.
	INSTRUCTION_TYPE type = instruction->getType();
	INSTRUCTIONS opCode = instruction->getOpCode();

	// Update A for LOAD and STORE inst.
	if (opCode == SW || opCode == LW) {
		A = instruction->getImmediate();
	}

	if (type == ITYPE) {
		// Read register1 value, if in Register Status,
		// Update Q else get value from register
		int register1 = instruction->getArg1();
		if (regStatus->isSet(register1)) {
			int ROBId = regStatus->get(register1);
			int ROBValue = rob->getValue(ROBId);
			if (ROBValue != ROBSlot::DEFAULT_VALUE) {
				Vj = ROBValue;
			} else {
				Qj = ROBId;
			}
		} else {
			Vj = registers->get(register1);
		}


		// For following instruction, registerT is used as 2nd argument
		if (opCode == SW || opCode == LW ||
                opCode == BEQ || opCode == BNE) {
			int register2 = instruction->getDestination();
			if (regStatus->isSet(register2)) {
				int ROBId = regStatus->get(register2);
				int ROBValue = rob->getValue(ROBId);
				if (ROBValue != ROBSlot::DEFAULT_VALUE) {
					Vk = ROBValue;
				} else {
					Qk = ROBId;
				}
			} else {
				Vk = registers->get(register2);
			}
		}
	} else if (type == RTYPE) {
		// BREAK and NOP shouldn't hit RS
		assert(opCode != NOP && opCode != BREAK);

		// Update Arg1 and Arg2 to Vj/Qj and Vk/Qk
		int register1 = instruction->getArg1();
		if (regStatus->isSet(register1)) {
			int ROBId = regStatus->get(register1);
			int ROBValue = rob->getValue(ROBId);
			if (ROBValue != ROBSlot::DEFAULT_VALUE) {
				Vj = ROBValue;
			} else {
				Qj = ROBId;
			}
		} else {
			Vj = registers->get(register1);
		}

		int register2 = instruction->getArg2();
		if (regStatus->isSet(register2)) {
			int ROBId = regStatus->get(register2);
			int ROBValue = rob->getValue(ROBId);
			if (ROBValue != ROBSlot::DEFAULT_VALUE) {
				Vk = ROBValue;
			} else {
				Qk = ROBId;
			}
		} else {
			Vk = registers->get(register2);
		}
	}   // J doesn't do any lookup.

	// create RS entry
	// index is unique for each entry.
	RSEntry* entry = new RSEntry(++index, instruction, Vj, Vk, Qj, Qk, A);

	// add to reservations.
	reservations.push_back(entry);

	return entry;
}

void ReservationStation::remove(int RSId) {
	int index = -1;

	// search index corresponding to RSId
	for (int i = 0; reservations.size(); i++) {
		if (reservations[i]->getRSId() == RSId) {
			index = i;
			break;
		}
	}

	// Should be present
	assert(index > -1);

	// Get the entry
	RSEntry* entry = reservations[index];

	// Erase entry
	reservations.erase(reservations.begin() + index);

	// Free memory
	delete entry;
}

void ReservationStation::updateFromCDB() {
	// Iterate through RSs.
	for (vector<RSEntry*>::iterator it = reservations.begin();
			it != reservations.end(); it++) {
		RSEntry* entry = *it;
        // Ignore inst to be executed in future cycles.
		Instruction* instruction = entry->getInstruction();
		int currentExecutionCycle = Executor::getExecutionCycle();
		if (instruction->getExecutionCycle() > currentExecutionCycle) {
			continue;
		}

		// Check Qj and Qk for each one of them
		// If not zero, check CDB
		// If value found, update corresponding V entry
		// Set Q entry to 0.
		int Qj = entry->getQj();
		if (Qj != RSEntry::DEFAULT_VALUE) {
			int value = cdb->get(Qj);
            int valueFromROB = rob->getValue(Qj);
            int register1 = instruction->getArg1();
			if (value != CDB::DEFAULT_VALUE) {
				entry->setVj(value);
				entry->resetQj();
            } else if (valueFromROB != ROBSlot::DEFAULT_VALUE) {
                entry->setVj(valueFromROB);
				entry->resetQj();
		    }
		}

		int Qk = entry->getQk();
		if (Qk != RSEntry::DEFAULT_VALUE) {
			int value = cdb->get(Qk);
            int valueFromROB = rob->getValue(Qk);
			if (value != CDB::DEFAULT_VALUE) {
				entry->setVk(value);
				entry->resetQk();
            } else if (valueFromROB != ROBSlot::DEFAULT_VALUE) {
                entry->setVk(valueFromROB);
				entry->resetQk();
			}
		}

	}
}


string ReservationStation::resStationDump() {
    stringstream ss;
    ss << "RS:";
	for (vector<RSEntry*>::iterator it = reservations.begin();
			it != reservations.end(); it++) {
		RSEntry* entry = *it;
        Instruction* inst = entry->getInstruction();

        ss << endl << "[" << inst->instructionString() << "] ";
    }
    return ss.str();
}
