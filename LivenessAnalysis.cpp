/*
 * LivenessAnalysis.cpp
 *
 *  Created on: Oct 9, 2009
 *      Author: yogesh
 */

#include "LivenessAnalysis.h"
#include "globals.h"
#include "string.h"

LivenessAnalysis::LivenessAnalysis(Instructions& inst) :
	instructions(inst)
{
	initProgramInfo();

}

LivenessAnalysis::~LivenessAnalysis()
{
	deleteOldLiveInfo();
}

void LivenessAnalysis::deleteOldLiveInfo()
{
	if (liveInfo != NULL)
	{
		for (int i = 0; i < noInstruction; i++)
		{
			delete liveInfo[i];
		}
		delete[] liveInfo;
		liveInfo = NULL;
	}
}

void LivenessAnalysis::analyse()
{
	initLivelinessInfo();
	calculateLiveliness();
}

void LivenessAnalysis::calculateLiveliness()
{
	bool changing = true;
	while (changing)
	{
		changing = false;
		for (int i = noInstruction - 1; i >= 0; i--)
		{
			if (liveInfo[i]->calculateLiveliness())
				changing = true;
		}
	}
}

void LivenessAnalysis::print()
{
	for (int i = 0; i < noInstruction; i++)
	{
		PRINTF("Instruction No %d - ",i+1 );
#ifdef VERBOSE
		instructions[i]->printInstruction(stdout, false);
#endif
		PRINTF(" ");
		liveInfo[i]->print();
		PRINTF("\n");
	}
}

void LivenessAnalysis::initLivelinessInfo()
{
	liveInfo = new LivenessInfo*[noInstruction];
	for (int i = 0; i < noInstruction; i++)
	{
		liveInfo[i] = new LivenessInfo(noRegisters, memorySize, minMemOffset);
	}

	int ctr = 0;

	// get label info
	for (InstructionsConstIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		inst_t inst = (*iter)->getInst();

		if (inst->label != NULL)
			labelMap[inst->label] = ctr;

		ctr++;
	}

	ctr = 0;
	for (InstructionsConstIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		inst_t inst = (*iter)->getInst();

		LivenessInfo *info = liveInfo[ctr];
		// create use def info
		createUseDefInfo(*info, inst);

		// create successor info
		createSuccessorInfo(*info, inst, ctr);

		// this is a last element
		if (iter == instructions.begin() + instructions.size()-1)
		{
			info->setLastInstruction();
		}

		ctr++;
	}
}

void LivenessAnalysis::createSuccessorInfo(LivenessInfo &info, inst_t inst,
		int index)
{
	int brLabelIndex, braLabelIndex;
	switch (inst->op)
	{
		case OP_BRA :
			braLabelIndex = labelMap[inst->ops[0].label];
			info.addSuccessor(*liveInfo[braLabelIndex]);
			break;
		case OP_BR :
			brLabelIndex = labelMap[inst->ops[1].label];
			info.addSuccessor(*liveInfo[brLabelIndex]);
		default :
			if (index + 1 < noInstruction)
				info.addSuccessor(*liveInfo[index + 1]);
			break;
	}

}

void LivenessAnalysis::createUseDefInfo(LivenessInfo &info, inst_t inst)
{
	int useReg1 = NO_REGISTER;
	int useReg2 = NO_REGISTER;
	int useReg3 = NO_REGISTER;
	switch (inst->op)
	{
		case OP_ADD :
		case OP_AND :
		case OP_ANDL :
		case OP_DIV :
		case OP_MUL :
		case OP_OR :
		case OP_ORL :
		case OP_SUB :
			info.setCanBeEliminated(true);
			info.setDefRegister(inst->ops[0].reg);
			useReg1 = inst->ops[1].reg;

			if (inst->ops[2].t == op_reg)
				useReg2 = inst->ops[2].reg;

			info.setUseRegisters(useReg1, useReg2);
			break;
		case OP_SET :
			info.setCanBeEliminated(true);
			info.setDefRegister(inst->ops[0].reg);
			break;
		case OP_NOT :
		case OP_NOTL :
			info.setCanBeEliminated(true);
			info.setDefRegister(inst->ops[0].reg);
			info.setUseRegisters(inst->ops[1].reg);
			break;
		case OP_LDR :
			info.setCanBeEliminated(true);
			info.setDefRegister(inst->ops[0].reg);
			if (inst->ops[1].reg == R5 && inst->ops[2].t == op_imm)
			{
				info.setUseMemory(memoryToOffset(inst->ops[2].imm));
				info.setUseRegisters(R5);
			}
			else
			{
				useReg1 = inst->ops[1].reg;
				if (inst->ops[2].t == op_reg)
					useReg2 = inst->ops[2].reg;

				info.setUseRegisters(useReg1, useReg2);
				info.set$UseRegister();
			}
			break;
		case OP_STR :
			info.setCanBeEliminated(true);
			useReg1 = inst->ops[0].reg;
			if (inst->ops[1].reg == R5 && inst->ops[2].t == op_imm)
			{
				info.setDefMemory(memoryToOffset(inst->ops[2].imm));
				info.setUseRegisters(R5);
			}
			else
			{
				useReg2 = inst->ops[1].reg;
				if (inst->ops[2].t == op_reg)
					useReg3 = inst->ops[2].reg;

				info.set$UseRegister();
				info.set$DefRegister();
			}
			info.setUseRegisters(useReg1, useReg2, useReg3);
			break;
		case OP_BR :
			info.setCanBeEliminated(false);
			info.setUseRegisters(inst->ops[0].reg);
			break;
		case OP_OUT :
			info.setCanBeEliminated(false);
			info.setUseRegisters(R0);
			break;
		case OP_IN :
			info.setCanBeEliminated(false);
			info.setDefRegister(R0);
			break;
		case OP_BRA :
		case OP_JSR :
		case OP_ST :
		case OP_STI :
		case OP_LD :
		case OP_LDI :
		case OP_LEA :
		case OP_JSRR :
		case OP_JMP :
		default :
			info.setCanBeEliminated(false);
			break;
	}
}

int LivenessAnalysis::memoryToOffset(int mem)
{
	return mem - minMemOffset;
}

void LivenessAnalysis::initProgramInfo()
{
	int ctr = 0;
	int maxRegister = REGS_MINSIZE;
	int minoffset = 0;
	int maxOffset = MEM_MINSIZE;

	for (InstructionsConstIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		inst_t inst = (*iter)->getInst();
		int reg = getHighestReg(inst);
		if (reg > maxRegister)
			maxRegister = reg;

		int offset = getR5Offset(inst);
		if (offset < minoffset)
			minoffset = offset;
		else if (offset > maxOffset)
			maxOffset = offset;

		ctr++;
	}

	noInstruction = ctr;
	noRegisters = maxRegister + 1;
	memorySize = (maxOffset - minoffset) + 1;
	minMemOffset = minoffset;
}

int LivenessAnalysis::getR5Offset(inst_t instruction)
{
	int memOffset = 0;
	if (instruction->ops[1].t == op_reg && instruction->ops[1].reg == R5)
	{
		if (instruction->ops[2].t == op_imm)
			memOffset = instruction->ops[2].imm;
	}
	return memOffset;
}

int LivenessAnalysis::getHighestReg(inst_t instruction)
{
	int highestReg = 0;
	for (int i = 0; i < 3; i++)
	{
		if (instruction->ops[i].t == op_reg)
		{
			if (instruction->ops[i].reg > highestReg)
				highestReg = instruction->ops[i].reg;
		}
	}
	return highestReg;
}

RegisterSet LivenessAnalysis::getLiveRegisters(int instNo)
{
	return liveInfo[instNo]->getLiveRegisters();
}

bool LivenessAnalysis::dce()
{
	int noErased=0;
	for (int i = 0; i < noInstruction; i++)
	{
		if(liveInfo[i]->isDeadCode())
		{
			PRINTF("Instruction eliminated");
			InstructionsIter iter = instructions.begin()+i-noErased;
			(*iter)->printInstruction(stdout);
			instructions.erase(instructions.begin()+i-noErased);
			noErased++;
		}
	}
	noInstruction -=noErased;
	if(noErased>0)
		return true;
	else
		return false;
}
