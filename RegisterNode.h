/*
 * RegisterNode.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#ifndef REGISTERNODE_H_
#define REGISTERNODE_H_

#include "globals.h"
#include "Instruction.h"
#include <set>
class RegisterNode
{
	Register reg;

	typedef std::set<Instruction*> RegisterUsageSet;
	typedef RegisterUsageSet::iterator RegisterUsageSetIter;

	typedef std::set<RegisterNode*> InterferingRegister;
	typedef InterferingRegister::iterator InterferingRegisterIter;

	RegisterUsageSet regUseSet;
	RegisterUsageSet regDefSet;
	InterferingRegister interferingRegs;

public:
	RegisterNode(Register reg);
	~RegisterNode();

	void addRegUse(Instruction& );
	void addRegDef(Instruction& );
	void addInterferingReg(RegisterNode& );

};

#endif /* REGISTERNODE_H_ */
