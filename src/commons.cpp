#include "commons.h"
int toInt(string bitString) {
    unsigned int val = 0;

    for (int i = 0; i < bitString.length(); i++) {
        val <<= 1;
        val |= ((int)bitString[i] - '0');
    }
    return val;
}


int fromTwosComplement(int num, int size) {
    int bitCheck = 1 << (size - 1);

    if (bitCheck & num) {
        int mask = (1 << (size)) - 1;
        return -1 * (((~ num) + 1) & mask);
    }
    return num;
}

unsigned int toTwosComplement(int num, int size) {
    if (num < 0) {
        num = num + 1;
        unsigned int mask = (1 << size) - 1;
        num = (~(-1 * num) & mask);
    }
    return num;
}

string toBitString(int num, int size) {
    stringstream ss;
    while (num > 0) {
        ss << (num & 1);
        num = num >> 1;
        size =- 1;
    }

    while (size > 0) {
        ss << 0;
        size =- 1;
    }
    string output = ss.str();
    reverse(output.begin(), output.end());
    return output;
}

/*
int main() {
    string str = "1111111111111000";
    cout << str << endl;
    cout << toInt(str) << endl;
    cout << twosComplement(toInt(str), 16) << endl;
    cout << toBitString(toTwosComplement(twosComplement(toInt(str), 16), 16), 16) << endl;
    cout << toBitString(toInt(str), 16) << endl;
    return 0;
}*/
