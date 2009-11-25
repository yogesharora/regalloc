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

void LiveRangeInfo::addInstruction(Instruction &inst,
		RegisterInfo &regToBeAdded)
{
	modified = true;

	int instructionNo = inst.getNo();
	const RegisterSet allRegs = inst.getAllRegisters();
	for (RegisterSetConstIter iter = allRegs.begin(); iter != allRegs.end(); iter++)
	{
		int reg = *iter;
		RegRange& range = regInfo[reg];
		if (range.end < instructionNo)
			range.end = instructionNo;
		if (range.start == -1)
			range.start = instructionNo;

		range.reg = &regToBeAdded;
	}

	int instNo = inst.getNo();
	if (instNo > maxInstNumber)
		maxInstNumber = inst.getNo();
	else if (instNo < minInstNumber)
		minInstNumber = instNo;
	if (minInstNumber == -1)
		minInstNumber = instNo;
}

void LiveRangeInfo::addRegister(RegisterInfo &reg)
{
	const RegisterInfo::RegisterUsageSet& regUse = reg.getUserInstructions();
	for (RegisterInfo::RegisterUsageSetConstIter iter = regUse.begin(); iter
			!= regUse.end(); iter++)
	{
		addInstruction(*(*iter), reg);
	}
}

const InterferenceGraph& LiveRangeInfo::getInterferenceGraph()
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
	}
	return *graph;
}
