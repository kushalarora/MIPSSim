#include<string>
#include "commons.h"
#include<sstream>

using namespace std;

#ifndef __MIPS_DATA__
#define __MIPS_DATA__
class Data {
private:
	string bitString;
	int value;
	int Address;
public:
	Data(int Address, string dataString) {
		this->Address = Address;
		bitString = dataString;
		value = fromTwosComplement(toInt(dataString), 32);
	}

	Data(int Address, int value) {
		this->Address = Address;
		this->value = value;
		bitString = toTwosComplement(value, 32);
	}

	string toString() {
		stringstream ss;
		ss << bitString << " " << Address << " " << value;
		return ss.str();
	}

	unsigned int getAddress() {
		return Address;
	}
	string getBitString() {
		return bitString;
	}
	int getValue() {
		return value;
	}
};
#endif
