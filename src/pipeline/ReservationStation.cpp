#include "ReservationStation.h"

RSEntry::RSEntry(int RSId, Instruction* instruction, int Vj, int Vk, int Qj, int Qk, unsigned int A) {
    this->instruction = instruction;
    this->RSId = RSId;
    this->Vj = Vj;
    this->Vk = Vk;
    this->Qj = Qj;
    this->Qk = Qk;
    this->A = A;
}


RSEntry* ReservationStation::add(Instruction* instruction) {
    assert (reservations.size() < MAX_SIZE);

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
            Qj = regStatus->get(register1);
        } else {
            Vj = registers[register1];
        }

        // For STORE update value to be stored as second operand
        if (opCode == SW) {
            int storeReg = instruction->getDestination();
            if (regStatus->isSet(storeReg)) {
                Qk = regStatus->get(storeReg);
            } else {
                Vk = registers[storeReg];
            }
        }
    } else if (type == RTYPE) {
        // BREAK and NOP shouldn't hit RS
        assert(opCode != NOP && opCode != BREAK);

        // Update Arg1 and Arg2 to Vj/Qj and Vk/Qk
        int register1 = instruction->getArg1();
        if (regStatus->isSet(register1)) {
            Qj = regStatus->get(register1);
        } else {
            Vj = registers[register1];
        }

        int register2 = instruction->getArg2();
        if (regStatus->isSet(register2)) {
            Qk = regStatus->get(register2);
        } else {
            Vk = registers[register2];
        }
    } else {
        // JUMP
        // TODO:: How to handle this?
    }

    // create RS entry
    // index is unique for each entry.
    RSEntry* entry = new RSEntry(++index, instruction,
                                Vj, Vk, Qj, Qk, A);

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
    assert (index > -1);

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
        // Check Qj and Qk for each one of them
        // If not zero, check CDB
        // If value found, update corresponding V entry
        // Set Q entry to 0.
        int Qj = entry->getQj();
        if (Qj != RSEntry::DEFAULT_VALUE) {
            int value = cdb->get(Qj);
            if (value != CDB::DEFAULT_VALUE) {
                entry->setVj(value);
                entry->resetQj();
            }
        }

        int Qk = entry->getQj();
        if (Qk != RSEntry::DEFAULT_VALUE) {
            int value = cdb->get(Qk);
            if (value != CDB::DEFAULT_VALUE) {
                entry->setVk(value);
                entry->resetQk();
            }
        }
    }

    // TODO: Figure out load store handling
}
