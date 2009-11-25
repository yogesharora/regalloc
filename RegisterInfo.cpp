/*
 * RegisterInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterInfo.h"

RegisterInfo::RegisterInfo() :
	reg(INVALID_REG)
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
}

void RegisterInfo::addRegDef(Instruction& inst, Register r)
{
	reg = r;
	regDefSet.insert(&inst);
	regAllSet.insert(&inst);
}

