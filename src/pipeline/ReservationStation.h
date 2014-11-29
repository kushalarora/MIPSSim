#include "../instructions/Instruction.h"
#include <vector>
#include <cassert>
#include <climits>
#include "CDB.h"
#include "RegisterStatus.h"
#include "Registers.h"
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
	int Qj; // ROBId for ROB entry of dependent instruction for 1st argument, 0 is Vj present
	int Qk; // ROBID for ROB entry of dependent instruction for 2nd argument, 0 if Vk present;
	unsigned int A;

public:
	const static int DEFAULT_VALUE;
	const static int DEFAULT_Q;
	int getRSId() {
		return RSId;
	}
	Instruction* getInstruction() {
		return instruction;
	}

	int getVj() {
		return Vj;
	}
	void setVj(int Vj) {
		this->Vj = Vj;
	}

	int getVk() {
		return Vk;
	}
	void setVk(int Vk) {
		this->Vk = Vk;
	}

	int getQj() {
		return Qj;
	}
	void resetQj() {
		this->Qj = DEFAULT_VALUE;
	}

	int getQk() {
		return Qk;
	}
	void resetQk() {
		this->Qk = DEFAULT_VALUE;
	}

	int getAddress() {
		return A;
	}
	void setAddress(unsigned address) {
		this->A = address;
	}

	bool isReady() {
		return Qj == DEFAULT_Q && Qk == DEFAULT_Q;
	}

	RSEntry(int RSId, Instruction* instruction, int Vj, int Vk, int Qj, int Qk,
			unsigned int A);
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
	Registers* registers;

public:
	static const int MAX_SIZE = 10;

	bool isFull() {
		return (reservations.size() == MAX_SIZE);
	}

	RSEntry* add(Instruction* instruction);

	void remove(int RSId);

	void flush() {
		reservations.clear();
	}

	void updateFromCDB();

    string resStationDump();

	vector<RSEntry*>& getEntries() {
		return reservations;
	}

    int size() {
        return reservations.size();
    }

	ReservationStation(CDB* cdb, RegisterStatus* regStatus,
			map<unsigned int, int>& SWAddToCount, Registers* registers) :
			SWAddToCount(SWAddToCount) {
		index = 0;
		this->cdb = cdb;
		this->registers = registers;
		this->regStatus = regStatus;
	}
};
#endif
