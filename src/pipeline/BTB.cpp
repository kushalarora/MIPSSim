#include "BTB.h"
unsigned int BranchTargetBuffer::getNextPC(unsigned int PC) {
    assert(PC >= BASE_PC);
    unsigned int nextPC = PC + 4;
    if (buffer.count(PC) > 0) {
        BTBEntry* btbEntry = buffer.at(PC);
        if (btbEntry->prediction) {
            nextPC = btbEntry->predictedPC;

            // Update the tick value to keep LRU behaviour
            assert(lastedTickedAt.count(PC) > 0);
            lastedTickedAt[PC]++;
        }
    }
    return nextPC;
}
