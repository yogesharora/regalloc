/*
 * RegisterAllocator.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterAllocator.h"

RegisterAllocator::RegisterAllocator(inst_t start) :
	instruction(start)
{
	initProgramInfo();
}

RegisterAllocator::~RegisterAllocator()
{
	for (InstructionsIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		delete *iter;
	}
}

void RegisterAllocator::initProgramInfo()
{
	inst_t cur = instruction;
	int ctr = 0;

	while (cur != NULL)
	{
		calcMaxMinRegisters(cur);
		Instruction* newInstruction = new Instruction(cur);
		updateInstructionInfo(*newInstruction);

		ctr++;
		cur = cur->next;
	}

	noOfInstructions = ctr;
	noOfRegisters = (maxReg - minReg) + 1;
}

void RegisterAllocator::updateInstructionInfo(Instruction& inst)
{
	liveRangeInfo.addInstruction(inst);
	instructions.reserve(inst.getNo()+1);
	instructions[inst.getNo()] = &inst;

	const RegisterSet &srcRegs = inst.getSrcRegisters();
	for (RegisterSetConstIter iter = srcRegs.begin(); iter != srcRegs.end(); iter++)
	{
		Register reg = *iter;
		registerInfo[reg].addRegUse(inst, reg);
	}

	int destReg = inst.getDestReg();
	if(destReg!=INVALID_REG)
		registerInfo[destReg].addRegDef(inst, destReg);
}

void RegisterAllocator::calcMaxMinRegisters(inst_t instruction)
{
	if (instruction->op == OP_IN || instruction->op == OP_OUT)
	{
		minReg = R0;
		if (R0 > maxReg)
			maxReg = R0;
	}
	for (int i = 0; i < 3; i++)
	{
		if (instruction->ops[i].t == op_reg)
		{
			if (minReg == INVALID_REG)
				minReg = instruction->ops[i].reg;

			if (instruction->ops[i].reg > maxReg)
				maxReg = instruction->ops[i].reg;
			else if (instruction->ops[i].reg < minReg)
				minReg = instruction->ops[i].reg;
		}
	}
}
