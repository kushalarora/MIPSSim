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
        bool prediction;    // prediction true = taken false = not taken
};

class BranchTargetBuffer {
    private:
        long long tick;
        map<int, BTBEntry*> buffer;     // pc to BTBEntry
        map<int, long long> lastedTickedAt;  // pc to last tick. Used to morph LRU behavior
    public:
        BranchTargetBuffer() {
            tick = 0;
        }

        void updateOrAdd(unsigned int PC, unsigned int nextPC, bool taken);
        unsigned int getNextPC(unsigned int PC);
};

#endif
