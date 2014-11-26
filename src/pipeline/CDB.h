#include <map>
#include <climits>
#include <cassert>

#ifndef __MIPS_CDB__
#define __MIPS_CDB__
using namespace std;
class CDB {

public:
	// value returned if result not in CDB
	static const int DEFAULT_VALUE = INT_MIN;

	int get(int ROBId);     // get value for ROBId
	void set(int ROBId, int value);  // set value for ROBId
	void clear(int ROBId);   // clear map for ROBId
	void flush();
	bool isPopulated(int ROBId);

private:
	map<int, int> ROBIdToValueMap;  // map for ROB id to value
};
#endif
