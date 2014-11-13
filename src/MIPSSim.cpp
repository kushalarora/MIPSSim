#include "MIPSSim.h"
#include <cassert>
#include "simulator/SimulatorBuilder.cpp"
#include "simulator/Simulator.h"
#include "instructions/InstructionBuilder.cpp"
#include "Data.cpp"
#include<fstream>
#include<sstream>


Arguments& readArgs(int argc, char** argv) {
    if(argc < 4) {
        cerr << "Not Enough Arguments(" << argc << ")";
    }

    Arguments* args = new Arguments;
    args->inputFileName = argv[1];
    args->outputFileName = argv[2];

    string opArg(argv[3]);
    if (opArg.compare("dis") == 0) {
        args->operation = DIS;
    } else if (opArg.compare("sim") == 0) {
        args->operation = SIM;
    } else {
        cerr << "Illegal Operation specified";
    }

    args->printAllCycle = true;
    if (argc > 4 ) {
        args->printAllCycle = false;

        string range(argv[4]);
        assert(string("-T").compare(range.substr(0, 2)));

        int colPosition = range.find(":");
        args->startCycle = range.substr(2, colPosition);
        args->endCycle = range.substr(colPosition + 1);
    }
    return *args;
}

bool readline(ifstream& file, string& str) {
    char* buffer = new char[4];
    file.read(buffer, 4);
    stringstream ss;
    char arr[32];
    for (int i = 0; i < 4; i++) {
        int num = (int)buffer[i];
        for (int j = 7; j >= 0; j--) {
            ss << (((num  >> j) & 1) ? '1' : '0');
        }
    }
    str = ss.str();
    return !file.eof();

}

int main(int argc, char* argv[]) {
    Arguments args = readArgs(argc, argv);
    Simulator* simulator = SimulatorBuilder::build(args.outputFileName, args.operation);

    ifstream infile;
    infile.open(args.inputFileName, ios::in | ios::binary);

    char buffer[4];
    string binaryString;
    bool isBreak = false;
    int lineNo = 600;
    while(readline(infile, binaryString)) {
        if (!isBreak) {
            Instruction* instruction = InstructionBuilder::build(lineNo, binaryString);
            simulator->addInstruction(instruction);
            isBreak = instruction->getOpCode() == BREAK;
        } else {
            simulator->addData(new Data(lineNo, binaryString));
        }
        lineNo += 4;
    }
    simulator->run();
}
