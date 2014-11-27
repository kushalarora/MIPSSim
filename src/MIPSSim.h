#include<iostream>
#include<fstream>
#include<string>
#include "commons.h"

using namespace std;

#ifndef __MIPSSIM_MIPSSIM__
#define __MIPSSIM__MIPSSIM__
struct Arguments {
	char* inputFileName;
	char* outputFileName;
	Operation operation;
	bool printAllCycle;
	string startCycle;
	string endCycle;
};
bool isSimulation(const Arguments& input);
bool isDisAssembly(const Arguments& input);
Arguments& readArgs(int argc, const char* argv[]);
#endif
