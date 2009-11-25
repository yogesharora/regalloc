/*
 * RegisterAllocator.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterAllocator.h"

RegisterAllocator::RegisterAllocator(inst_t start) :
	instruction(start),  maxReg(INVALID_REG), minReg(INVALID_REG)
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
		Instruction* newInstruction = new Instruction(cur, ctr);
		updateRegisterInfo(*newInstruction);

		ctr++;
		cur = cur->next;
	}

	for(RegistersIter iter=registerInfo.begin(); iter!=registerInfo.end(); iter++)
	{
		if(isAllocatableRegister(iter->second.getNo()))
			liveRangeInfo.addRegister(iter->second);
	}
	noOfInstructions = ctr;
	noOfRegisters = (maxReg - minReg) + 1;
}

bool RegisterAllocator::isAllocatableRegister(Register no)
{
	switch (no){
		case R0:
		case R4:
		case R5:
		case R6:
		case R7:
			return false;
			break;
		default:
			return true;
	}
}
void RegisterAllocator::updateRegisterInfo(Instruction& inst)
{
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

void RegisterAllocator::allocateRegs()
{
	liveRangeInfo.getInterferenceGraph();
}
