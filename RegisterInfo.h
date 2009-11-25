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
public:
	typedef std::set<Instruction*> RegisterUsageSet;
	typedef RegisterUsageSet::iterator RegisterUsageSetIter;
	typedef RegisterUsageSet::const_iterator RegisterUsageSetConstIter;

	RegisterInfo();
	~RegisterInfo();

	void addRegUse(Instruction&, Register r);
	void addRegDef(Instruction&, Register r);
	const RegisterUsageSet& getUserInstructions()
	{
		return regAllSet;
	}

private:
	Register reg;

	RegisterUsageSet regUseSet;
	RegisterUsageSet regDefSet;
	RegisterUsageSet regAllSet;

};

#endif /* REGISTERNODE_H_ */
