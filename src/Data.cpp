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
        int lineNo;
    public:
        Data(int lineNo, string dataString) {
            this->lineNo = lineNo;
            bitString = dataString;
            value = twosComplement(toInt(dataString), 32);
        }

        string toString() {
            stringstream ss;
            ss << bitString << " " << lineNo << " " << value;
            return ss.str();
        }
};
#endif
