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
	Instruction(inst_t inst);

	~Instruction();

	void renameRegister(Register from, Register to);

	void printInstruction(FILE* fptr);

	Register getDestReg()
	{
		return destReg;
	}

	const RegisterSet& getSrcRegisters()
	{
		return srcReg;
	}

	const RegisterSet& getAllRegisters()
	{
		return allReg;
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
	RegisterSet allReg;
};

#endif /* INSTRUCTION_H_ */
