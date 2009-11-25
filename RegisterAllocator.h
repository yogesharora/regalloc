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
#include "RegisterInfo.h"
#include "LiveRangeInfo.h"
#include <vector>
#include <map>

class RegisterAllocator
{
	inst_t instruction;
	int noOfInstructions;
	int noOfRegisters;
	Register maxReg;
	Register minReg;
	LiveRangeInfo liveRangeInfo;

	typedef std::vector<Instruction* > Instructions;
	typedef Instructions::iterator InstructionsIter;
	Instructions instructions;

	typedef std::map<Register, RegisterInfo> Registers;
	typedef Registers::iterator RegistersIter;
	Registers registerInfo;

	void initProgramInfo();
	void calcMaxMinRegisters(inst_t instruction);
	void updateRegisterInfo(Instruction&);
	bool isAllocatableRegister(Register no);

public:
	RegisterAllocator(inst_t start);
	~RegisterAllocator();
	void allocateRegs();
};

#endif /* REGISTERALLOCATOR_H_ */
