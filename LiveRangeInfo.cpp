/*
 * LiveRangeInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "globals.h"
#include "LiveRangeInfo.h"

LiveRangeInfo::LiveRangeInfo()
{
}

LiveRangeInfo::~LiveRangeInfo()
{
}

void LiveRangeInfo::addInstruction(Instruction &inst)
{
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
	}
}

void LiveRangeInfo::addRegister(RegisterInfo &reg)
{
	const RegisterInfo::RegisterUsageSet& regUse = reg.getUserInstructions();
	for (RegisterInfo::RegisterUsageSetConstIter iter = regUse.begin(); iter
			!= regUse.end(); iter++)
	{
		addInstruction(*(*iter));
	}
}
