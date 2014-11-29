#include "MIPSSim.h"
#include <cassert>
#include "simulator/SimulatorBuilder.cpp"
#include "simulator/Simulator.h"
#include "Data.cpp"
#include<fstream>
#include<sstream>

Arguments& readArgs(int argc, char** argv) {
	if (argc < 3) {
		cerr << "Not Enough Arguments(" << argc << ")";
	}

	Arguments* args = new Arguments;
	args->inputFileName = argv[1];
	args->outputFileName = argv[2];
	args->operation = SIM; // Hard coding as this argument will not be passed from the command line

	args->printAllCycle = true;
	if (argc > 3) {
		args->printAllCycle = false;

		string range(argv[3]);
		assert(string("-T").compare(range.substr(0, 2)) == 0);

		int colPosition = range.find(":");
		args->startCycle = atoi(range.substr(2, colPosition - 2).c_str());
		args->endCycle = atoi(range.substr(colPosition + 1).c_str());
	} else {
        args->startCycle = -1;
        args->endCycle = -1;
    }
	return *args;
}

bool readline(ifstream& file, string& str) {
	char* buffer = new char[4];
	file.read(buffer, 4);
	stringstream ss;
	char arr[32];
	for (int i = 0; i < 4; i++) {
		int num = (int) buffer[i];
		for (int j = 7; j >= 0; j--) {
			ss << (((num >> j) & 1) ? '1' : '0');
		}
	}
	str = ss.str();
	return !file.eof();

}

int main(int argc, char* argv[]) {
	Arguments args = readArgs(argc, argv);
	Simulator* simulator = SimulatorBuilder::build(args.outputFileName,
			args.operation, args.startCycle, args.endCycle);

	ifstream infile;
	infile.open(args.inputFileName, ios::in | ios::binary);

	char buffer[4];
	string binaryString;
	int PC = 600;
	while (readline(infile, binaryString)) {
		simulator->addToMemory(new Data(PC, binaryString));
		PC += 4;
	}
	simulator->run();
}
