#include<string>
#include "../commons.h"
using namespace std;

#ifndef __MIPS_RAW_INST__
#define __MIPS_RAW_INST__
class RawInstruction {
private:
	unsigned int address;
	string bitString;
	unsigned int decodeCycle;

public:
	unsigned int getAddress() {
		return address;
	}
	string getBitString() {
		return bitString;
	}
	unsigned int getDecodeCycle() {
		return decodeCycle;
	}

	RawInstruction(Data* data, unsigned int decodeCycle) {
		assert(decodeCycle >= BASE_PC);
		address = data->getAddress();
		bitString = data->getBitString();
		this->decodeCycle = decodeCycle;
	}
};
#endif
