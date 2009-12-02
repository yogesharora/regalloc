/*
 * LiveRangeInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "globals.h"
#include "LiveRangeInfo.h"

LiveRangeInfo::LiveRangeInfo(Instructions &inst,  Registers& reg) :
	instructions(inst), liveAnalysis(inst), registers(reg), graph(reg)
{
	liveAnalysis.analyse();
	liveAnalysis.print();
}

LiveRangeInfo::~LiveRangeInfo()
{
}

InterferenceGraph& LiveRangeInfo::getInterferenceGraph()
{
	for (InstructionsIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		RegisterSet regs = liveAnalysis.getLiveRegisters((*iter)->getNo());
		for (RegisterSetIter regIter = regs.begin(); regIter != regs.end(); regIter++)
		{
			RegisterInfo& v1 = *(registers[*regIter]);
			for (RegisterSetIter interferenceIter = regIter + 1; interferenceIter
					!= regs.end(); interferenceIter++)
			{
				RegisterInfo& v2 = *(registers[*interferenceIter]);
				graph.addInterference(v1, v2);
			}
		}
	}

	return graph;
}

