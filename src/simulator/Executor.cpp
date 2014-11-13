#include "Simulator.h"
class Executor : public Simulator {
    public:
        Executor(char* logFileName) : Simulator(logFileName) {}
        void run() {
            cout << "Executor Running";
        }
};
