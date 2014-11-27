#include "OpcodeMap.h"

map<string, INSTRUCTIONS> OpcodeMap::buildMap() {
	map<string, INSTRUCTIONS> mp;
	mp["101011"] = SW;
	mp["100011"] = LW;
	mp["000100"] = BEQ;
	mp["000101"] = BNE;
	mp["000001"] = BGEZ;
	mp["000111"] = BGTZ;
	mp["000110"] = BLEZ;
	mp["000001"] = BLTZ;
	mp["001000"] = ADDI;
	mp["001001"] = ADDIU;
	mp["001010"] = SLTI;
	mp["000000"] = SLTU;
	mp["000000"] = SLL;
	mp["000000"] = SRL;
	mp["000000"] = SRA;
	mp["000000"] = SUB;
	mp["000000"] = SUBU;
	mp["000000"] = ADD;
	mp["000000"] = ADDU;
	mp["000000"] = AND;
	mp["000000"] = OR;
	mp["000000"] = XOR;
	mp["000000"] = NOR;
	mp["000000"] = NOP;
	mp["000000"] = SLT;
	mp["000010"] = J;
	mp["000000"] = BREAK;

	return mp;
}
map<string, INSTRUCTIONS> OpcodeMap::strToOpCodeMap = buildMap();

map<string, INSTRUCTIONS> OpcodeMap::buildFunctionMap() {
	map<string, INSTRUCTIONS> mp;
	mp["101011"] = SLTU;
	mp["000000"] = SLL;
	mp["000010"] = SRL;
	mp["000011"] = SRA;
	mp["100010"] = SUB;
	mp["100011"] = SUBU;
	mp["100000"] = ADD;
	mp["100001"] = ADDU;
	mp["100100"] = AND;
	mp["100101"] = OR;
	mp["100110"] = XOR;
	mp["100111"] = NOR;
	mp["101010"] = SLT;
	mp["001101"] = BREAK;

	return mp;
}

map<string, INSTRUCTIONS> OpcodeMap::functionToOpCodeMap = buildFunctionMap();
