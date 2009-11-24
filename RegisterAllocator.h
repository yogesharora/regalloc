/*
 * RegisterAllocator.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#ifndef REGISTERALLOCATOR_H_
#define REGISTERALLOCATOR_H_

#include "globals.h"
#include "s3inst.h"
#include "Instruction.h"
#include "RegisterNode.h"
#include <vector>

class RegisterAllocator
{
	inst_t instruction;
	int noOfInstructions;
	int noOfRegisters;
	Register maxReg;
	Register minReg;

	typedef std::vector<Instruction* > Instructions;
	typedef Instructions::iterator InstructionsIter;
	Instructions instructions;

	void initProgramInfo();
	void calcMaxMinRegisters(inst_t instruction);
	void updateLiveRangeInfo(Instruction&);

public:
	RegisterAllocator(inst_t start);
	~RegisterAllocator();
};

#endif /* REGISTERALLOCATOR_H_ */
