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
#include "InterferenceGraph.h"
#include "RegisterInfo.h"
#include "typedefs.h"
#include <vector>
#include <map>
#include <set>

class LiveRangeInfo
{
	struct RegRange {
		int start;
		int end;
		RegisterInfo *reg;

		RegRange() : start(-1), end(-1) {}
	};

	typedef std::map<Register, RegRange> RegRangeInfo;
	typedef RegRangeInfo::iterator RegRangeInfoIter;

	typedef std::set<RegisterInfo*> RegisterInfoSet;
	typedef RegisterInfoSet::iterator RegisterInfoSetIter;
	typedef std::vector<RegisterInfoSet> RegRanges;

	RegRangeInfo regInfo;

	InterferenceGraph graph;

	LiveRangeInfo(LiveRangeInfo&);

	Instructions &instructions;
public:
	LiveRangeInfo(Instructions &instructions);
	~LiveRangeInfo();
	InterferenceGraph& getInterferenceGraph();
};

#endif /* LIVERANGEINFO_H_ */
