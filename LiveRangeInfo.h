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
#include "LivenessAnalysis.h"
#include "typedefs.h"
#include <vector>
#include <map>
#include <set>

class LiveRangeInfo
{
	LiveRangeInfo(LiveRangeInfo&);

	Instructions &instructions;
	LivenessAnalysis liveAnalysis;
	Registers &registers;
	InterferenceGraph graph;

public:
	LiveRangeInfo(Instructions &instructions, Registers& registers);
	~LiveRangeInfo();
	InterferenceGraph& getInterferenceGraph();
};

#endif /* LIVERANGEINFO_H_ */
