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
	cost +=2;
}

void RegisterInfo::addRegDef(Instruction& inst)
{
	regDefSet.insert(&inst);
	regAllSet.insert(&inst);
	cost+=1;
}

