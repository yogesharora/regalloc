/*
 * LiveRangeInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "globals.h"
#include "LiveRangeInfo.h"

LiveRangeInfo::LiveRangeInfo(Instructions &inst,  Registers& reg, Register origMax) :
	instructions(inst), liveAnalysis(inst), registers(reg), graph(reg, origMax), origMaxReg(origMax)
{
	liveAnalysis.analyse();
}

LiveRangeInfo::~LiveRangeInfo()
{
}

InterferenceGraph& LiveRangeInfo::getInterferenceGraph()
{
	for (InstructionsIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		Register dest = (*iter)->getDestReg();
		bool destInLiveout = false;
		RegisterSet regs = liveAnalysis.getLiveRegisters((*iter)->getNo());
		for (RegisterSetIter regIter = regs.begin(); regIter != regs.end(); regIter++)
		{
			RegisterInfo& v1 = *(registers[*regIter]);
			if(v1.getNo() == dest)
			{
				destInLiveout = true;
			}

			for (RegisterSetIter interferenceIter = regIter + 1; interferenceIter
					!= regs.end(); interferenceIter++)
			{
				Register v2Reg = *interferenceIter;
				RegisterInfo& v2 = *(registers[*interferenceIter]);
				graph.addInterference(v1, v2);
			}
		}

		// also add interference from the destination register to the live out of instruction

		if (dest != INVALID_REG && destInLiveout==false)
		{
			RegisterSet regs = liveAnalysis.getLiveRegisters((*iter)->getNo());
			RegisterInfo& destVertex = *(registers[dest]);
			for (RegisterSetIter regIter = regs.begin(); regIter != regs.end(); regIter++)
			{
				RegisterInfo& liveOutVertex = *(registers[*regIter]);
				if(liveOutVertex.getNo() != dest)
				{
					graph.addInterference(destVertex, liveOutVertex);
				}
			}
		}
	}

	return graph;
}

