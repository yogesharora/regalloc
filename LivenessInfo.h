/*
 * LivenessInfo.h
 *
 *  Created on: Oct 8, 2009
 *      Author: yogesh
 */

#ifndef LIVENESSINFO_H_
#define LIVENESSINFO_H_

#include "Bitset.h"
#include <vector>
#include "globals.h"

class LivenessInfo
{
	int noRegisters;
	int memorySize;
	int minMemory;
	Bitset use;
	Bitset def;
	Bitset liveIn;
	Bitset liveOut;
	bool canBeEliminated;
	bool last;
	int defRegister;

	typedef std::vector<LivenessInfo*> SuccesorVector;
	SuccesorVector successor;
	LivenessInfo(LivenessInfo&);

public:
	LivenessInfo(int noOfRegsister, int memSize, int minMem);
	~LivenessInfo();
	void setUseRegisters(int regNo1, int regNo2=NO_REGISTER, int reg3=NO_REGISTER);
	void setDefRegister(int regNo);
	void setUseMemory(int memory);
	void setDefMemory(int memory);
	void set$DefRegister();
	void set$UseRegister();
	void setLastInstruction();
	void addSuccessor(LivenessInfo&);
	void setCanBeEliminated(bool yesNo) { canBeEliminated=yesNo; }
	int getCanBeEliminated() {return canBeEliminated;}
	void print();
	// returns true if liveness has changed
	bool calculateLiveliness();
	bool isDeadCode();
};

#endif /* LIVENESSINFO_H_ */
