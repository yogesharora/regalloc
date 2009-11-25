/*
 * RegisterInfo.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#ifndef REGISTERNODE_H_
#define REGISTERNODE_H_

#include "globals.h"
#include "Instruction.h"
#include <set>

class RegisterInfo
{
	Register reg;

	typedef std::set<Instruction*> RegisterUsageSet;
	typedef RegisterUsageSet::iterator RegisterUsageSetIter;

	RegisterUsageSet regUseSet;
	RegisterUsageSet regDefSet;

public:
	RegisterInfo();
	~RegisterInfo();

	void addRegUse(Instruction& , Register r);
	void addRegDef(Instruction& , Register r);
};

#endif /* REGISTERNODE_H_ */
