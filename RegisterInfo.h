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

	RegisterInfo(Register reg);
	~RegisterInfo();

	void addRegUse(Instruction&);
	void addRegDef(Instruction&);
	const RegisterUsageSet& getUserInstructions()
	{
		return regAllSet;
	}

	Register getNo() const
	{
		return reg;
	}

	int getCost() const
	{
		return cost;
	}

private:
	RegisterInfo(const RegisterInfo&);
	Register reg;

	int cost;

	RegisterUsageSet regUseSet;
	RegisterUsageSet regDefSet;
	RegisterUsageSet regAllSet;

};

#endif /* REGISTERNODE_H_ */
