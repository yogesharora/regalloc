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
	Instruction(inst_t inst, int no, bool del=false);

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

	void setNo(int no)
	{
		instructionNumber = no;
	}

	inst_t getInst() const
	{
		return instruction;
	}

	Instruction& fillInst(int reg, int newReg,int r5Offset);

	Instruction& spillInst(int reg, int newReg, int r5Offset);

private:
	Instruction(Instruction&);
	void initRegisterInfo();
	void printOperand(FILE *fptr, struct operand op);
	void printConditionCode(FILE *fptr, int ccode);
	void renameSrcRegister(Register from, Register to);
	void renameDestRegister(Register from, Register to);
	inst_t genFillInst(int newReg, int no);
	inst_t genSpillInst(int newReg, int no);

	inst_t instruction;
	int instructionNumber;
	int destReg;
	bool toDelete;
	RegisterSet srcReg;
	RegisterSet allReg;
};

#endif /* INSTRUCTION_H_ */
