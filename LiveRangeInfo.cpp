/*
 * LiveRangeInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "globals.h"
#include "LiveRangeInfo.h"

LiveRangeInfo::LiveRangeInfo() :
	graph(NULL), modified(false), minInstNumber(-1), maxInstNumber(-1)
{
}

LiveRangeInfo::~LiveRangeInfo()
{
}

void LiveRangeInfo::addRegister(RegisterInfo &reg)
{
	modified =  true;
	const RegisterInfo::RegisterUsageSet& regUse = reg.getUserInstructions();
	for (RegisterInfo::RegisterUsageSetConstIter iter = regUse.begin(); iter
			!= regUse.end(); iter++)
	{
		int instNo = (*iter)->getNo();
		RegRange& range = regInfo[reg.getNo()];
		if (range.end < instNo)
			range.end = instNo;
		if (range.start == -1)
			range.start = instNo;

		range.reg = &reg;

		if (instNo > maxInstNumber)
			maxInstNumber = instNo;
		else if (instNo < minInstNumber)
			minInstNumber = instNo;
		if (minInstNumber == -1)
			minInstNumber = instNo;
	}
}

InterferenceGraph& LiveRangeInfo::getInterferenceGraph()
{
	if (modified)
	{
		delete graph;
		graph = new InterferenceGraph();

		// build interference info
		int instructionSize = maxInstNumber - minInstNumber + 1;
		RegRanges liveRange(instructionSize);
		for (int i = 0; i < instructionSize; i++)
		{
			liveRange.push_back(RegisterInfoSet());
		}

		for (RegRangeInfoIter iter = regInfo.begin(); iter != regInfo.end(); iter++)
		{
			RegRange &range = iter->second;
			liveRange[range.start - minInstNumber].insert(range.reg);
			liveRange[range.end - minInstNumber].insert(range.reg);
		}

		// build graph
		for (RegRangeInfoIter iter = regInfo.begin(); iter != regInfo.end(); iter++)
		{
			RegRange &range = iter->second;
			for (int i = range.start + 1; i < range.end; i++)
			{
				RegisterInfoSet& interferingRegs = liveRange[i - minInstNumber];
				for (RegisterInfoSetIter iter2 = interferingRegs.begin(); iter2
						!= interferingRegs.end(); iter2++)
				{
					graph->addInterference(*range.reg, **iter2);
				}
			}
		}

		// create register queues
		graph->createRegisterQueues();
	}
	return *graph;
}

