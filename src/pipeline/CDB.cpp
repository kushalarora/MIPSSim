#include "CDB.h"

int CDB::get(int ROBId) {
	if (ROBIdToValueMap.count(ROBId) == 0) {
		return DEFAULT_VALUE;
	}
	return ROBIdToValueMap[ROBId];
}

void CDB::set(int ROBId, int value) {
	ROBIdToValueMap[ROBId] = value;
}

void CDB::clear(int ROBId) {
	assert(ROBIdToValueMap.count(ROBId) > 0);

	ROBIdToValueMap.erase(ROBId);
}

void CDB::flush() {
	ROBIdToValueMap.clear();
}

bool CDB::isPopulated(int ROBId) {
	return ROBIdToValueMap.count(ROBId) > 0
			&& ROBIdToValueMap[ROBId] != DEFAULT_VALUE;
}
