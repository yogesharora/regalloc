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
	Instruction(inst_t inst, int no);

	~Instruction();

	void renameRegister(Register from, Register to);

	void printInstruction(FILE* fptr);

	Register getDestReg() const
	{
		return destReg;
	}

	const RegisterSet& getSrcRegisters() const
	{
		return srcReg;
	}

	const RegisterSet& getAllRegisters() const
	{
		return allReg;
	}

	int getNo() const
	{
		return instructionNumber;
	}


private:
	Instruction(Instruction&);
	void initRegisterInfo();
	void printOperand(FILE *fptr, struct operand op);
	void printConditionCode(FILE *fptr, int ccode);

	inst_t instruction;
	int instructionNumber;
	int destReg;
	bool rootNode;
	RegisterSet srcReg;
	RegisterSet allReg;
};

#endif /* INSTRUCTION_H_ */
