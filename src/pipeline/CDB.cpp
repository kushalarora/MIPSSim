#include "CDB.h"
#include "../simulator/Executor.h"

int CDB::get(int ROBId) {
	if (ROBIdToValueMap.count(ROBId) ==  0) {
        return DEFAULT_VALUE;
    }

    CDBEntry* entry = ROBIdToValueMap[ROBId];
    if (entry->executionCycle == Executor::getExecutionCycle()) {
        return entry->value;
    }
    return DEFAULT_VALUE;
}

void CDB::set(int ROBId, int value) {
    CDBEntry* entry = new CDBEntry();
    entry->value = value;
    entry->executionCycle = Executor::getExecutionCycle() + 1;
	ROBIdToValueMap[ROBId] = entry;
}

void CDB::clear(int ROBId) {
	assert(ROBIdToValueMap.count(ROBId) > 0);
	ROBIdToValueMap.erase(ROBId);
}

void CDB::flush() {
    map<int, CDBEntry*>::iterator it = ROBIdToValueMap.begin();
    while (it != ROBIdToValueMap.end()) {
        CDBEntry* entry = it->second;
        if (Executor::getExecutionCycle() == entry->executionCycle)  {
            ROBIdToValueMap.erase(it++);
        } else {
            it++;
        }
    }
}

bool CDB::isPopulated(int ROBId) {
	return ROBIdToValueMap.count(ROBId) > 0;
}
