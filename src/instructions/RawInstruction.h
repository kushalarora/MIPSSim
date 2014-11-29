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
    bool outcome;

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

    void setOutCome(bool outcome) {
        this->outcome = outcome;
    }

    bool getOutCome() {
        return outcome;
    }

	RawInstruction(Data* data, unsigned int decodeCycle) {
        outcome = false;
		address = data->getAddress();
		bitString = data->getBitString();
		this->decodeCycle = decodeCycle;
	}
};
#endif
