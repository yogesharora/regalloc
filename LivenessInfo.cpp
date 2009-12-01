/*
 * LivenessInfo.cpp
 *
 *  Created on: Oct 8, 2009
 *      Author: yogesh
 */

#include "LivenessInfo.h"
#include "globals.h"

LivenessInfo::LivenessInfo(int noOfRegsister, int memSize, int minMem) :
	noRegisters(noOfRegsister), memorySize(memSize), minMemory(minMem), use(
			noOfRegsister + memSize + 1), def(noOfRegsister + memSize + 1),
			liveIn(noOfRegsister + memSize + 1), liveOut(noOfRegsister
					+ memSize + 1), last(false), defRegister(NO_REGISTER)
{
}

LivenessInfo::~LivenessInfo()
{
}

void LivenessInfo::setUseRegisters(int regNo1, int regNo2, int regNo3)
{
	if (regNo1 >= 0 && regNo1 < noRegisters)
		use[regNo1] = BIT_ON;
	if (regNo2 >= 0 && regNo2 < noRegisters)
		use[regNo2] = BIT_ON;
	if (regNo3 >= 0 && regNo3 < noRegisters)
		use[regNo3] = BIT_ON;
}

void LivenessInfo::setDefRegister(int regNo)
{
	if (regNo >= 0 && regNo < noRegisters)
	{
		def[regNo] = BIT_ON;
		defRegister = regNo;
	}

}

void LivenessInfo::setUseMemory(int memory)
{
	if (memory >= 0 && memory < memorySize)
		use[noRegisters + memory] = BIT_ON;
}

void LivenessInfo::setDefMemory(int memory)
{
	if (memory >= 0 && memory < memorySize)
	{
		def[noRegisters + memory] = BIT_ON;
		defRegister = noRegisters + memory;
	}
}
void LivenessInfo::addSuccessor(LivenessInfo& succ)
{
	successor.push_back(&succ);
}

bool LivenessInfo::calculateLiveliness()
{
	Bitset oldRegLiveOut = liveOut;
	Bitset oldRegLiveIn = liveIn;
	bool modified = false;

	Bitset emptyRegSet(noRegisters + memorySize + 1);

	if (!last)
		liveOut = emptyRegSet;

	for (SuccesorVector::iterator itr = successor.begin(); itr
			!= successor.end(); itr++)
	{
		LivenessInfo &succ = *(*itr);
		liveOut |= succ.liveIn;
	}

	liveIn = use | (liveOut - def);

	modified = !((liveIn == oldRegLiveIn) && (liveOut == oldRegLiveOut));
	return modified;
}
void LivenessInfo::set$DefRegister()
{
	def[noRegisters + memorySize] = BIT_ON;
	defRegister = noRegisters + memorySize;
}

void LivenessInfo::set$UseRegister()
{
	use[noRegisters + memorySize] = BIT_ON;
}

void LivenessInfo::setLastInstruction()
{
	last = true;
	liveOut[R4] = BIT_ON;
	liveOut[R5] = BIT_ON;
	liveOut[R6] = BIT_ON;
	liveOut[R7] = BIT_ON;
	liveOut[noRegisters + LIVEOUT_MEM - minMemory] = BIT_ON;
	liveOut[noRegisters + memorySize] = BIT_ON;
}

void LivenessInfo::print()
{
//	PRINTF("use - {");
//	use.print("R%d ", 0, noRegisters);
//	use.print("Mem[R5%d] ", noRegisters, noRegisters + (0 - minMemory),
//			minMemory);
//	use.print("Mem[R5+%d] ", noRegisters + (0 - minMemory), noRegisters
//			+ memorySize, 0);
//	use.print("$ ", noRegisters + memorySize, noRegisters + memorySize + 1);
//	PRINTF("} ");
//
//	PRINTF("def - {");
//	def.print("R%d ", 0, noRegisters);
//	def.print("Mem[R5%d] ", noRegisters, noRegisters + (0 - minMemory),
//			minMemory);
//	def.print("Mem[R5+%d] ", noRegisters + (0 - minMemory), noRegisters
//			+ memorySize, 0);
//	def.print("$ ", noRegisters + memorySize, noRegisters + memorySize + 1, 0);
//	PRINTF("} ");

	PRINTF("liveout - {");
	RegisterSet reg;
	for(int i=0;i<noRegisters;i++)
	{
		if(isAllocatableRegister(i))
		{
			if(liveOut[i]==BIT_ON)
				PRINTF("R%d ", i);
		}
	}
//		return reg;
//	liveOut.print("R%d ", 0, noRegisters);
	PRINTF("} ");

//	PRINTF("livein - {");
//	liveIn.print("R%d ", 0, noRegisters);
//	liveIn.print("Mem[R5%d] ", noRegisters, noRegisters + (0 - minMemory),
//			minMemory);
//	liveIn.print("Mem[R5+%d] ", noRegisters + (0 - minMemory), noRegisters
//			+ memorySize, 0);
//	liveIn.print("$ ", noRegisters + memorySize, noRegisters + memorySize + 1,
//			0);
//	PRINTF("}");
}

bool LivenessInfo::isDeadCode()
{
	if (canBeEliminated && defRegister != NO_REGISTER)
	{
		if (liveOut[defRegister] == BIT_OFF)
			return true;
	}
	return false;
}

RegisterSet LivenessInfo::getLiveRegisters()
{
	RegisterSet reg;
	for(int i=0;i<noRegisters;i++)
	{
		if(isAllocatableRegister(i))
		{
			if(liveOut[i]==BIT_ON)
				reg.push_back(i);
		}
	}
	return reg;
}

bool LivenessInfo::isAllocatableRegister(Register no)
{
	switch (no)
	{
		case R0 :
		case R4 :
		case R5 :
		case R6 :
		case R7 :
			return false;
			break;
		default :
			return true;
	}
}
