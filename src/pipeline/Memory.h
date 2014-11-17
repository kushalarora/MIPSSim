#include<vector>
#include<cassert>
#include<string>
using namespace std;

#ifndef __MIPS_MEMORY__
#define __MIPS_MEMORY__

class Memory {
    private:
        vector<string> memory;
    public:
        static int const START_ADDRESS = 600;
        string read(unsigned int address) {
            return memory.at(address - START_ADDRESS);
        }

        string write(unsigned int address, string value) {
            assert(address >= START_ADDRESS);
            memory[address - START_ADDRESS] = value;
        }
};

#endif
