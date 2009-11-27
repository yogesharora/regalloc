/*
 * RegisterInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterInfo.h"

RegisterInfo::RegisterInfo(Register r) :
	reg(r), cost(0)
{
}

RegisterInfo::~RegisterInfo()
{
}

void RegisterInfo::addRegUse(Instruction& inst)
{
	regUseSet.insert(&inst);
	regAllSet.insert(&inst);
	cost = 2 * regUseSet.size() + regDefSet.size();
}

void RegisterInfo::addRegDef(Instruction& inst)
{
	regDefSet.insert(&inst);
	regAllSet.insert(&inst);
	cost = 2 * regUseSet.size() + regDefSet.size();
}

void RegisterInfo::assignRegisterToInstructions(Register newReg)
{
	if (reg != newReg)
	{
		for (RegisterInfo::RegisterUsageSetIter iter = regAllSet.begin(); iter
				!= regAllSet.end(); iter++)
		{
			Instruction& inst = *(*iter);

			inst.renameRegister(reg, newReg);
		}
	}
}
