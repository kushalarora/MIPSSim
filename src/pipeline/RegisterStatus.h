#include<climits>
class RegisterStatus {
    private:
       int registerStatus[32];
    public:
       RegisterStatus() {
           for (int i = 0; i < 32; i++) {
               registerStatus[i] = INT_MIN;
           }
       }

       int get(int index) { return registerStatus[index]; }
       void set(int index, int value) { registerStatus[index] = value; }
       void flush() {
           for (int i = 0; i < 32; i++) {
               registerStatus[i] = INT_MIN;
           }
       }
};
