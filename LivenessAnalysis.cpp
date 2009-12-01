/*
 * livenessAnalysis.cpp
 *
 *  Created on: Oct 9, 2009
 *      Author: yogesh
 */

#include "LivenessAnalysis.h"
#include "globals.h"
#include "string.h"

livenessAnalysis::livenessAnalysis(Instruction& inst)
{
	initProgramInfo();

}

livenessAnalysis::~livenessAnalysis()
{
	deleteOldLiveInfo();
}

void livenessAnalysis::deleteOldLiveInfo()
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

void livenessAnalysis::analyse()
{
	initLivelinessInfo();
	calculateLiveliness();
}


void livenessAnalysis::calculateLiveliness()
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

void livenessAnalysis::print()
{
	for (int i = 0; i < noInstruction; i++)
	{
		PRINTF("Instruction No %d - ",i+1 );
		liveInfo[i]->print();
		PRINTF("\n");
	}
}

void livenessAnalysis::initLivelinessInfo()
{
////	liveInfo = new livenessInfo*[noInstruction];
////	for (int i = 0; i < noInstruction; i++)
////	{
////		liveInfo[i] = new livenessInfo(noRegisters, memorySize, minMemOffset);
////	}
////
////	inst_t head = instructions;
////	int ctr = 0;
////
////	// get label info
////	while (head)
////	{
////		if (head->label != NULL)
////			labelMap[head->label] = ctr;
////
////		head = head->next;
////		ctr++;
////	}
////	head = instructions;
////
////	 ctr = 0;
////	while (head)
////	{
////		livenessInfo *info = liveInfo[ctr];
////		// create use def info
////		createUseDefInfo(*info, head);
////
////		// create successor info
////		createSuccessorInfo(*info, head, ctr);
////
////		//this is a last element
////		if (head->next == NULL)
////		{
////			info->setLastInstruction();
////		}
////
////		head = head->next;
////		ctr++;
//	}
}

void livenessAnalysis::createSuccessorInfo(LivenessInfo &info, inst_t inst,
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

void livenessAnalysis::createUseDefInfo(LivenessInfo &info, inst_t inst)
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

int livenessAnalysis::memoryToOffset(int mem)
{
	return mem - minMemOffset;
}

void livenessAnalysis::initProgramInfo()
{
	inst_t head = instructions;
	int ctr = 0;
	int maxRegister = REGS_MINSIZE;
	int minoffset = 0;
	int maxOffset = MEM_MINSIZE;

	while (head)
	{
		int reg = getHighestReg(head);
		if (reg > maxRegister)
			maxRegister = reg;

		int offset = getR5Offset(head);
		if (offset < minoffset)
			minoffset = offset;
		else if (offset > maxOffset)
			maxOffset = offset;

		head = head->next;
		ctr++;
	}

	noInstruction = ctr;
	noRegisters = maxRegister + 1;
	memorySize = (maxOffset - minoffset) + 1;
	minMemOffset = minoffset;
}

int livenessAnalysis::getR5Offset(inst_t instruction)
{
	int memOffset = 0;
	if (instruction->ops[1].t == op_reg && instruction->ops[1].reg == R5)
	{
		if (instruction->ops[2].t == op_imm)
			memOffset = instruction->ops[2].imm;
	}
	return memOffset;
}

int livenessAnalysis::getHighestReg(inst_t instruction)
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
