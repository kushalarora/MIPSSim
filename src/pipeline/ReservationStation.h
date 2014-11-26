#include "../instructions/Instruction.h"
#include <vector>
#include <cassert>
#include <climits>
#include "CDB.h"
#include "RegisterStatus.h"
#include <map>

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
        const static int DEFAULT_VALUE = INT_MIN;
        const static int DEFAULT_Q = 0;
        int getRSId() { return RSId;}
        Instruction* getInstruction() { return instruction; }
        
        int getVj() { return Vj; }
        void setVj(int Vj) { this->Vj = Vj; }

        int getVk() { return Vk; }
        void setVk(int Vk) { this->Vk = Vk; }

        int getQj() { return Qj; }
        void resetQj() { this->Qj = DEFAULT_VALUE; }

        int getQk() { return Qk; }
        void resetQk() { this->Qk = DEFAULT_VALUE; }

        int getAddress() { return A; }
        void setAddress(unsigned address) { 
            this->A = address;
        }

        RSEntry(int RSId, Instruction* instruction, int Vj, int Vk, int Qj, int Qk, unsigned int A);
};

// Reservation Station. It is a collection of upto MaxSize reservation
// station entries for each execution unit.
class ReservationStation {
    private:

        vector<RSEntry*> reservations;
        int index;
        CDB* cdb;
        RegisterStatus* regStatus;
        map<unsigned int, int>& SWAddToCount;
        int* registers;

    public:
        static const int MAX_SIZE = 10;

        bool isFull() {reservations.size() == MAX_SIZE;}

        RSEntry* add(Instruction* instruction);

        void remove(int RSId);

        void flush() { reservations.clear(); }

        void updateFromCDB();
        
        void updateAddForLDSW();

        ReservationStation(CDB* cdb, RegisterStatus* regStatus, map<unsigned int, int>& SWAddToCount, int* registers)
            : SWAddToCount(SWAddToCount) {
            index = 0;
            this->cdb = cdb;
            this->registers = registers;
            this->regStatus = regStatus;
        }
};
#endif
