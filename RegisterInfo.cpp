/*
 * RegisterInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterInfo.h"

RegisterInfo::RegisterInfo() :
	reg(INVALID_REG), cost(0)
{
}

RegisterInfo::~RegisterInfo()
{
}

void RegisterInfo::addRegUse(Instruction& inst, Register r)
{
	reg = r;
	regUseSet.insert(&inst);
	regAllSet.insert(&inst);
	cost +=2;
}

void RegisterInfo::addRegDef(Instruction& inst, Register r)
{
	reg = r;
	regDefSet.insert(&inst);
	regAllSet.insert(&inst);
	cost+=1;
}

