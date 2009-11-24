/*
 * RegisterNode.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterNode.h"

RegisterNode::RegisterNode(Register r) :
	reg(r)
{
}

RegisterNode::~RegisterNode()
{
}

void RegisterNode::addRegUse(Instruction& inst)
{
	regUseSet.insert(&inst);
}

void RegisterNode::addRegDef(Instruction& inst)
{
	regDefSet.insert(&inst);
}

void RegisterNode::addInterferingReg(RegisterNode& reg)
{
	interferingRegs.insert(&reg);
}
