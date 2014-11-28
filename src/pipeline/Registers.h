#include <climits>
#include "../commons.h"
#ifndef __MIPS_REGS__
#define __MIPS_REGS__
using namespace std;
class Registers {
    private:
        int reg[32];
    public:
        Registers() {
            for (int i = 0; i < 32; i++) {
                reg[i] = INT_MIN;
            }
        }
        int get(int regNo) {
            assert (regNo >= 0 && regNo < 32);
            cout << regNo <<endl;
            return reg[regNo];
        }

        void set(int regNo, int value) {
            assert (regNo >= 0 && regNo < 32);
            reg[regNo] = value;
        }

        string registersDump() {
           stringstream ss;
           ss << "Registers:";
           for (int i = 0; i < 32; i++) {
                // Add "RXX:" for modulo 8 registers
                if (i % 8 == 0) {
                    ss << endl << "R";
                    // Extra 0 padding
                    if (i < 10) {
                        ss << "0";
                    }
                    ss << i << ":";
                }
                ss << " " << reg[i]; 
            }
            return ss.str();
        }
};
#endif
