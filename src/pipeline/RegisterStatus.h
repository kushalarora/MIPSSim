#include<climits>


#ifndef __MIPS_REG_STATUS__
#define __MIPS_REG_STATUS__
class RegisterStatus {
    private:
       int registerStatus[32];

    public:
       const int DEFAULT_VALUE = INT_MIN;
       RegisterStatus() {
           for (int i = 0; i < 32; i++) {
               registerStatus[i] = DEFAULT_VALUE;
           }
       }

       int get(int index) {
           return registerStatus[index];
       }

       void set(int index, int value) {
           registerStatus[index] = value;
       }

       void flush() {
           for (int i = 0; i < 32; i++) {
               registerStatus[i] = DEFAULT_VALUE;
           }
       }
};
#endif
