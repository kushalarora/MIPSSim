#include<cassert>

#ifndef __MIPS_REGISTER__
#define __MIPS_REGISTER__

class Register {
    private:
        unsigned int regstr[32];     // 32 register of type int
    public:
        unsigned int read(int regstrIdx) {
            assert(regstrIdx > -1 && regstrIdx < 32);
            return regstr[regstrIdx];
        }

        void write(int regstrIdx, unsigned int value) {
            assert(regstrIdx > -1 && regstrIdx < 32);
            regstr[regstrIdx] = value;
        }
};


#endif
