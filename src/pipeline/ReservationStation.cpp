#include "ReservationStation.h"

RSEntry::RSEntry(int RSId, Instruction* instruction) {
    this->instruction = instruction;
    this->RSId = RSId;

    // TODO:: Code to check registerstatus and update Vj, Vk, Qj, Qk.
    // TODO:: Destination and Address fields to be populated.
}


RSEntry* ReservationStation::add(Instruction* instruction) {
    assert (reservations.size() < MAX_SIZE);

    // create RS entry
    // index is unique for each entry.
    RSEntry* entry = new RSEntry(++index, instruction);

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
