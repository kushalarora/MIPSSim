#include<iostream>
#include<string>
#include<sstream>
#include<algorithm>
#include<cassert>
#ifndef __MIPSSIM_COMMONS__
#define __MIPSSIM_COMMONS__
using namespace std;
enum Operation {
	DIS, SIM
};

const unsigned int BASE_PC = 600;

enum INSTRUCTION_TYPE {
	ITYPE, RTYPE, JTYPE
};

enum INSTRUCTIONS {
	SW,
	LW,
	J,
	BEQ,
	BNE,
	BGEZ,
	BGTZ,
	BLEZ,
	BLTZ,
	ADDI,
	ADDIU,
	BREAK,
	SLT,
	SLTI,
	SLTU,
	SLL,
	SRL,
	SRA,
	SUB,
	SUBU,
	ADD,
	ADDU,
	AND,
	OR,
	XOR,
	NOR,
	NOP
};

static const char* OpCodeStrings[] = { "SW", "LW", "J", "BEQ", "BNE", "BGEZ",
		"BGTZ", "BLEZ", "BLTZ", "ADDI", "ADDIU", "BREAK", "SLT", "SLTI", "SLTU",
		"SLL", "SRL", "SRA", "SUB", "SUBU", "ADD", "ADDU", "AND", "OR", "XOR",
		"NOR", "NOP" };

enum STATE {
	ISSUE, EXECUTE, WRITE_RESULT, COMMIT
};

int toInt(string bitString);
int fromTwosComplement(int num, int size);
unsigned int toTwosComplement(int num, int size);
string toBitString(int num, int size);
#endif
