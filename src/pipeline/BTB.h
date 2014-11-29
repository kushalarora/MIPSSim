#include<cassert>
#include<map>
#include<string>
#include<cassert>
#include "../commons.h"

using namespace std;

#ifndef __MIPS_BTB__
#define __MIPS_BTB__

class BTBEntry {
public:
    static const int NOT_SET;
    static const int TAKEN;
    static const int NOT_TAKEN;
	unsigned int predictedPC;    // predicted program counter
	int outcome;    // outcome == 1 => taken, outcome == 0 => not taken,
                    // outcome == -1 => not set

public:
	BTBEntry(unsigned int predictedPC, int outcome) {
		this->predictedPC = predictedPC;
		this->outcome = outcome;
	}
};

class BranchTargetBuffer {
private:
	long long tick;
	map<int, BTBEntry*> buffer;     // pc to BTBEntry
	map<int, long> lastedTickedAt; // pc to last tick. Used to morph LRU behavior
	static const int MAXSIZE = 16;	// maximum size of BTB
	int size;	//Current size of BTB
public:
	BranchTargetBuffer() {
		tick = 0;
		size = 0;
	}

	void updateOrAdd(unsigned int pc, int outcome);
	unsigned int getNextPC(unsigned int PC, unsigned int nextPC);

    string btbDump();
};
#endif
