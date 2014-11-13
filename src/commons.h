#include<iostream>
#include<string>

#ifndef __MIPSSIM_COMMONS__
#define __MIPSSIM_COMMONS__
using namespace std;
enum Operation {DIS, SIM};

enum INSTRUCTIONS {
    SW, LW, \
    J, BEQ, BNE, BGEZ, BGTZ, BLEZ, BLTZ,
    ADDI, ADDIU,
    BREAK,
    SLT, SLTI, SLTU,
    SLL, SRL, SRA,
    SUB, SUBU, ADD, ADDU,
    AND, OR, XOR, NOR,
    NOP
};

static const char* OpCodeStrings[] = {
    "SW", "LW",
    "J", "BEQ", "BNE", "BGEZ", "BGTZ", "BLEZ", "BLTZ",
    "ADDI", "ADDIU",
    "BREAK",
    "SLT", "SLTI", "SLTU",
    "SLL", "SRL", "SRA",
    "SUB", "SUBU", "ADD", "ADDU",
    "AND", "OR", "XOR", "NOR",
    "NOP"
};

int toInt(string bitString);
int twosComplement(int num, int size);
#endif
