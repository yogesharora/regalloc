/*
 * Instruction.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#ifndef INSTRUCTION_H_
#define INSTRUCTION_H_

#include "s3inst.h"
#include "globals.h"
#include <set>

class Instruction
{

public:
	typedef std::set<Register> RegisterSet;
	typedef RegisterSet::iterator RegisterSetIter;
	typedef RegisterSet::const_iterator RegisterSetConstIter;

	Instruction(inst_t inst);

	~Instruction();

	void renameRegister(Register from, Register to);

	void printInstruction(FILE* fptr);

	Register getDestReg()
	{
		return destReg;
	}

	const Instruction::RegisterSet& getSrcRegisters()
	{
		return srcReg;
	}

	int getNo()
	{
		return instructionNumber;
	}


private:
	void initRegisterInfo();
	void printOperand(FILE *fptr, struct operand op);
	void printConditionCode(FILE *fptr, int ccode);

	inst_t instruction;
	int instructionNumber;
	int destReg;
	bool rootNode;
	RegisterSet srcReg;
};

#endif /* INSTRUCTION_H_ */
