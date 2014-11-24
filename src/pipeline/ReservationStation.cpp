#include "ReservationStation.h"

RSEntry::RSEntry(int index, Instruction* instruction) {
    this->instruction = instruction;
    this->index = index;
    
    // TODO:: Code to check registerstatus and update Vj, Vk, Qj, Qk.
    // TODO:: Destination and Address fields to be populated.
}


RSEntry* ReservationStation::add(Instruction* instruction) {
    assert (reservations.size() < MAX_SIZE);
    
    // Adding at the back
    int index = reservations.size();

    RSEntry* entry = new RSEntry(index, instruction);
    reservations.push_back(entry);
    return entry;
}


void ReservationStation::remove(int index) {
    assert(index >= 0 && index < reservations.size());
    RSEntry* entry = reservations[index];
    reservations.erase(reservations.begin() + index);
    delete entry;
}
