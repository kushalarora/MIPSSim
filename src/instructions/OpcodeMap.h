#include<iostream>
#include "../commons.h"
#include<map>

#ifndef __MIPSSIM_OPCODEMAP__
#define __MIPSSIM_OPCODEMAP__
class OpcodeMap {
public:
	static map<string, INSTRUCTIONS> strToOpCodeMap;
	static map<string, INSTRUCTIONS> functionToOpCodeMap;
	static map<string, INSTRUCTIONS> buildMap();
	static map<string, INSTRUCTIONS> buildFunctionMap();
};
#endif
