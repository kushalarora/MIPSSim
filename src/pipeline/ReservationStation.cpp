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
