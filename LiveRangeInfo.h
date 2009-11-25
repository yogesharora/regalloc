/*
 * LiveRangeInfo.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#ifndef LIVERANGEINFO_H_
#define LIVERANGEINFO_H_

#include "globals.h"
#include "Instruction.h"
#include "RegisterInfo.h"
#include <vector>
#include <map>

class LiveRangeInfo
{
	struct RegRange {
		int start;
		int end;

		RegRange() : start(-1), end(-1) {}
	};

	typedef std::map<Register, RegRange> RegRangeInfo;
	RegRangeInfo regInfo;

public:
	LiveRangeInfo();
	~LiveRangeInfo();
	void addInstruction(Instruction &);
	void addRegister(RegisterInfo &);
};

#endif /* LIVERANGEINFO_H_ */
