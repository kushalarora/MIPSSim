#include "commons.h"
int toInt(string bitString) {
    unsigned int val = 0;

    for (int i = 0; i < bitString.length(); i++) {
        val <<= 1;
        val |= ((int)bitString[i] - '0');
    }
    return val;
}


int twosComplement(int num, int size) {
    int bitCheck = 1 << (size - 1);

    if (bitCheck & num) {
        int mask = (1 << (size)) - 1;
        return -1 * (((~ num) + 1) & mask);
    }
    return num;

}
