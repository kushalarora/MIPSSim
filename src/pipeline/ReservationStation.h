#include "../instructions/Instruction.h"
#include<vector>
#include<cassert>

using namespace std;

#ifndef __MIPS_RES_STATION__
#define __MIPS_RES_STATION__
// Reservation station entry
class RSEntry {
    private:
        int RSId;      // RS Entry ID
        Instruction* instruction;   // Instruction Reference
        int Vj;     // Value of 1st argument
        int Vk;     // Value of 2nd argument
        int Qj;     // ROBId for ROB entry of dependent instruction for 1st argument, 0 is Vj present
        int Qk;     // ROBID for ROB entry of dependent instruction for 2nd argument, O if Vk present;
        unsigned int A;
    public:
        int getRSId() { return RSId;}
        Instruction* getInstruction() { return instruction; }
        int getVj() { return Vj; }
        int getVk() { return Vk; }
        int getQj() { return Qj; }
        int getQk() { return Qk; }
        int getAddress() { return A; }
        RSEntry(int index, Instruction* instruction);
};

// Reservation Station. It is a collection of upto MaxSize reservation
// station entries for each execution unit.
class ReservationStation {
    private:
        vector<RSEntry*> reservations;
        int index;
    public:
        static const int MAX_SIZE = 10;
        bool isFull() {reservations.size() == MAX_SIZE;}
        RSEntry* add(Instruction* instruction);
        void remove(int RSId);
        void flush() { reservations.clear(); }
        ReservationStation() {
            index = 0;
        }
};
#endif
