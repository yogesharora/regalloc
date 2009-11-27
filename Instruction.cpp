/*
 * Instruction.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "Instruction.h"

Instruction::Instruction(inst_t inst, int no, bool del) :
	instruction(inst), instructionNumber(no), destReg(INVALID_REG), toDelete(del)
{
	initRegisterInfo();
}

Instruction::~Instruction()
{
	if(toDelete)
	{
		delete instruction;
	}
}

Instruction& Instruction::fillInst(int reg, int newReg, int r5Offset)
{
	renameSrcRegister(reg, newReg);
	allReg.clear();
	srcReg.clear();
	destReg = INVALID_REG;
	initRegisterInfo();
	inst_t fill = genFillInst(newReg, r5Offset);
	if(instruction->label!=NULL)
	{
		fill->label = instruction->label;
		instruction->label = NULL;
	}
	Instruction *fillInst = new Instruction(fill, instructionNumber-1, true);
	return *fillInst;
}

inst_t Instruction::genFillInst(int newReg, int r5Offset)
{
	inst_t fill = new inst_d;
	fill->op = OP_LDR;
	fill->ops[0].t = op_reg;
	fill->ops[0].reg = newReg;
	fill->ops[1].t = op_reg;
	fill->ops[1].reg = R5;
	fill->ops[2].t = op_imm;
	fill->ops[2].reg = r5Offset;
	fill->label = NULL;
	return fill;
}

Instruction& Instruction::spillInst(int reg, int newReg, int r5Offset)
{
	renameDestRegister(reg, newReg);
	allReg.clear();
	srcReg.clear();
	destReg = INVALID_REG;
	initRegisterInfo();
	inst_t spill = genSpillInst(newReg, r5Offset);
	Instruction *spillInst = new Instruction(spill, instructionNumber+1, true);
	return *spillInst;
}

inst_t Instruction::genSpillInst(int oldReg, int r5Offset)
{
	inst_t spill = new inst_d;
	spill->op = OP_STR;
	spill->ops[0].t = op_reg;
	spill->ops[0].reg = oldReg;
	spill->ops[1].t = op_reg;
	spill->ops[1].reg = R5;
	spill->ops[2].t = op_imm;
	spill->ops[2].reg = r5Offset;
	spill->label = NULL;
	return spill;
}


void Instruction::renameSrcRegister(Register from, Register to)
{
	switch (instruction->op)
	{
		case OP_ADD :
		case OP_AND :
		case OP_ANDL :
		case OP_DIV :
		case OP_MUL :
		case OP_OR :
		case OP_ORL :
		case OP_SUB :
		case OP_LDR :
			if (instruction->ops[1].reg == from)
				instruction->ops[1].reg = to;
			if (instruction->ops[2].reg == from)
				instruction->ops[2].reg = to;
			break;
		case OP_NOT :
		case OP_NOTL :
		case OP_LDI :
			if (instruction->ops[1].reg == from)
				instruction->ops[1].reg = to;
			break;
		case OP_ST :
		case OP_STI :
		case OP_BR :
			if (instruction->ops[0].reg == from)
				instruction->ops[0].reg = to;
			break;
		case OP_STR :
			if (instruction->ops[0].reg == from)
				instruction->ops[0].reg = to;
			if (instruction->ops[1].reg == from)
				instruction->ops[1].reg = to;
			if (instruction->ops[2].reg == from)
				instruction->ops[2].reg = to;
			break;
		default :
			break;
	}
}

void Instruction::renameDestRegister(Register from, Register to)
{
	switch (instruction->op)
	{
		case OP_ADD :
		case OP_AND :
		case OP_ANDL :
		case OP_DIV :
		case OP_MUL :
		case OP_OR :
		case OP_ORL :
		case OP_SUB :
		case OP_SET :
		case OP_NOT :
		case OP_NOTL :
		case OP_LD :
		case OP_LDI :
		case OP_LDR :
		case OP_LEA :
			if (instruction->ops[0].reg == from)
				instruction->ops[0].reg = to;
		default :
			break;
	}
	if (destReg == from)
		destReg = to;
}

void Instruction::initRegisterInfo()
{
	switch (instruction->op)
	{
		case OP_ADD :
		case OP_AND :
		case OP_ANDL :
		case OP_DIV :
		case OP_MUL :
		case OP_OR :
		case OP_ORL :
		case OP_SUB :
			destReg = instruction->ops[0].reg;
			srcReg.push_back(instruction->ops[1].reg);

			if (instruction->ops[2].t == op_reg)
				srcReg.push_back(instruction->ops[2].reg);
			break;
		case OP_SET :
			destReg = instruction->ops[0].reg;
			break;
		case OP_NOT :
		case OP_NOTL :
			destReg = instruction->ops[0].reg;
			srcReg.push_back(instruction->ops[1].reg);
			break;
		case OP_LD :
			destReg = instruction->ops[0].reg;
			break;
		case OP_LDI :
			destReg = instruction->ops[0].reg;
			srcReg.push_back(instruction->ops[1].reg);
			break;
		case OP_LDR :
			destReg = instruction->ops[0].reg;
			srcReg.push_back(instruction->ops[1].reg);
			if (instruction->ops[2].t == op_reg)
				srcReg.push_back(instruction->ops[2].reg);
			break;
		case OP_ST :
			srcReg.push_back(instruction->ops[0].reg);
			break;
		case OP_STI :
			srcReg.push_back(instruction->ops[0].reg);
			break;
		case OP_STR :
			srcReg.push_back(instruction->ops[0].reg);
			if (instruction->ops[1].t == op_reg)
				srcReg.push_back(instruction->ops[1].reg);
			if (instruction->ops[2].t == op_reg)
				srcReg.push_back(instruction->ops[2].reg);
			break;
		case OP_BR :
			srcReg.push_back(instruction->ops[0].reg);
			break;
		case OP_LEA :
			destReg = instruction->ops[0].reg;
			break;
		case OP_OUT :
			srcReg.push_back(R0);
			break;
		case OP_IN :
			destReg = R0;
			break;

		case OP_BRA :
		case OP_JSR :
		case OP_JSRR :
		case OP_JMP :
		default :
			break;
	}

	if (destReg != INVALID_REG)
		allReg.push_back(destReg);

	for (RegisterSetIter iter = srcReg.begin(); iter != srcReg.end(); iter++)
		allReg.push_back(*iter);
}

void Instruction::renameRegister(Register from, Register to)
{
	for (int i = 0; i < 3; i++)
	{
		if (instruction->ops[i].t == op_reg && instruction->ops[i].reg == from)
		{
			instruction->ops[i].reg = to;

		}
	}
}

void Instruction::printInstruction(FILE* fptr)
{
	if (instruction->label)
	{
		fprintf(fptr, "%s:", instruction->label);
	}

	if (instruction->op == OP_BR)
	{
		fprintf(fptr, "\t%s", opnames[instruction->op]);
		printConditionCode(fptr, instruction->ccode);
	}
	else
		fprintf(fptr, "\t%s ", opnames[instruction->op]);

	switch (instruction->op)
	{

		/* 3 operands */
		case OP_ADD :
		case OP_AND :
		case OP_ANDL :
		case OP_DIV :
		case OP_LDR :
		case OP_MUL :
		case OP_OR :
		case OP_ORL :
		case OP_STR :
		case OP_SUB :
			printOperand(fptr, instruction->ops[0]);
			fprintf(fptr, ", ");
			printOperand(fptr, instruction->ops[1]);
			fprintf(fptr, ", ");
			printOperand(fptr, instruction->ops[2]);
			break;
			/* 2 operands */
		case OP_BR :
		case OP_SET :
		case OP_ST :
		case OP_STI :
		case OP_LD :
		case OP_LDI :
		case OP_LEA :
		case OP_NOT :
		case OP_NOTL :
			printOperand(fptr, instruction->ops[0]);
			fprintf(fptr, ", ");
			printOperand(fptr, instruction->ops[1]);
			break;

			/* one operand */
		case OP_JSRR :
		case OP_BRA :
		case OP_JMP :
		case OP_JSR :
			printOperand(fptr, instruction->ops[0]);

		default :
			break;
	}
	fprintf(fptr, "\n");
}

void Instruction::printConditionCode(FILE *fptr, int ccode)
{
	if (ccode & CC_N)
		fprintf(fptr, "n");
	if (ccode & CC_Z)
		fprintf(fptr, "z");
	if (ccode & CC_P)
		fprintf(fptr, "p");

	fprintf(fptr, " ");
}

void Instruction::printOperand(FILE *fptr, struct operand op)
{
	enum op_type t = op.t;
	switch (t)
	{
		case op_reg :
			fprintf(fptr, "R%d", op.reg);
			break;
		case op_imm :
			fprintf(fptr, "#%d", op.imm);
			break;
		case op_label :
			fprintf(fptr, "%s", op.label);
			break;
	}
}
