#include "../instructions/Instruction.h"

using namespace std;

#ifndef __MIPS_RES_STATION__
#define __MIPS_RES_STATION__
// Reservation station entry
class RSEntry {
    private:
        int index;
        Instruction& instruction;
        int Vj;
        int Vk;
        int Qj;
        int Qk;
        unsigned int destination;
        unsigned int A;
    public:
        int getIndex() { return index;}
        Instruction& getInstruction() { return instruction; }
        int getVj() { return Vj; }
        int getVk() { return Vk; }
        int getQj() { return Qj; }
        int getQk() { return Qk; }
        int getDestination() { return destination; }
        int getAddress() { return A; }
};

// Reservation Station. It is a collection of upto MaxSize reservation
// station entries for each execution unit.
class ReservationStation {
    private:
    public:

};


#endif
