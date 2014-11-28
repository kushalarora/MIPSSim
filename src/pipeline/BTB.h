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
	unsigned int predictedPC;    // predicted program counter
	bool taken;    // taken true = taken false = not taken

public:
	BTBEntry(unsigned int predictedPC, bool taken) {
		this->predictedPC = predictedPC;
		this->taken = taken;
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

	void updateOrAdd(unsigned int pc, unsigned int nextPC, bool taken);
	unsigned int getNextPC(unsigned int PC);
};
#endif
