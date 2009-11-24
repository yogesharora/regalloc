/*
 * RegisterAllocator.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterAllocator.h"

RegisterAllocator::RegisterAllocator(inst_t start)
{

}

RegisterAllocator::~RegisterAllocator()
{
	// TODO Auto-generated destructor stub
}

void RegisterAllocator::initProgramInfo()
{
	inst_t cur = instruction;
	int ctr = 0;

	while (cur != NULL)
	{
		calcMaxMinRegisters(cur);
		Instruction* newInstruction = new Instruction(cur);
		instructions.push_back(newInstruction);
		//
		ctr++;
		cur = cur->next;
	}

	noOfInstructions = ctr;
	noOfRegisters = (maxReg-minReg) + 1;
}


void RegisterAllocator::calcMaxMinRegisters(inst_t instruction)
{
	if(instruction->op==OP_IN || instruction->op==OP_OUT)
	{
		minReg = R0;
		if(R0 > maxReg)
			maxReg = R0;
	}
	for (int i = 0; i < 3; i++)
	{
		if (instruction->ops[i].t == op_reg)
		{
			if(minReg==INVALID_REG)
				minReg = instruction->ops[i].reg;

			if (instruction->ops[i].reg > maxReg)
				maxReg = instruction->ops[i].reg;
			else if(instruction->ops[i].reg < minReg)
				minReg = instruction->ops[i].reg;
		}
	}
}
