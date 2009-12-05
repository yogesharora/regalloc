/*
 * RegisterAllocator.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterAllocator.h"
#include <stack>

extern int verbose;

using namespace std;
RegisterAllocator::RegisterAllocator(inst_t start) :
	instruction(start), maxReg(INVALID_REG), originalMaxReg(INVALID_REG),
			minReg(INVALID_REG)
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
	for (RegistersIter iter = registerInfo.begin(); iter != registerInfo.end(); iter++)
	{
		delete iter->second;
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
		instructions.push_back(newInstruction);
		updateRegisterInfo(*newInstruction);

		ctr++;
		cur = cur->next;
	}

	noOfInstructions = instructions.size();
	noOfRegisters = (maxReg - minReg) + 1;
	originalMaxReg = maxReg;
}

bool RegisterAllocator::isAllocatableRegister(Register no)
{
	switch (no)
	{
		case R0 :
		case R4 :
		case R5 :
		case R6 :
		case R7 :
			return false;
			break;
		default :
			return true;
	}
}

void RegisterAllocator::updateRegisterInfo(Instruction& inst)
{
	const RegisterSet &srcRegs = inst.getSrcRegisters();
	for (RegisterSetConstIter iter = srcRegs.begin(); iter != srcRegs.end(); iter++)
	{
		Register reg = *iter;
		if (registerInfo[reg] == NULL)
		{
			RegisterInfo *regInfo = new RegisterInfo(reg);
			registerInfo[reg] = regInfo;
		}
		registerInfo[reg]->addRegUse(inst);
	}

	int destReg = inst.getDestReg();
	if (destReg != INVALID_REG)
	{
		if (registerInfo[destReg] == NULL)
		{
			RegisterInfo *regInfo = new RegisterInfo(destReg);
			registerInfo[destReg] = regInfo;
		}
		registerInfo[destReg]->addRegDef(inst);
	}
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

void RegisterAllocator::verbosePrint(InterferenceGraph & graph)
{
	if (verbose)
	{
		MappingIter iter = mapping.begin();
		for (; iter != mapping.end(); iter++)
		{
			int from = iter->first;
			if (from > originalMaxReg)
			{
				break;
			}
			int to = iter->second;
			if (to > 0)
				fprintf(stdout, "%d: %d", from, to);
			else if (to < 0)
				fprintf(stdout, "%d: Mem[R5%d]", from, to);
			else
				fprintf(stdout, "%d: Mem[R5+0]", from);

			fprintf(stdout, "\n");


		}

		fprintf(stdout, "registers created after spill fill\n");
		for (; iter != mapping.end(); iter++)
		{
			int from = iter->first;
			int to = iter->second;
			if (to > 0)
				fprintf(stdout, "%d: %d", from, to);
			else if (to < 0)
				fprintf(stdout, "%d: Mem[R5%d]", from, to);
			else
				fprintf(stdout, "%d: Mem[R5+0]", from);

			fprintf(stdout, "\n");
		}
	}
}

bool RegisterAllocator::allocateRegs(Register startReg, int noOfPhysicalRegs,
		int noOfSpills)
{
	bool allAllocated = false;
	int spillCount = 0;
	do
	{
		// calulate liveness and live ranges
		LiveRangeInfo liveRangeInfo(instructions, registerInfo);

		// build interference graph
		InterferenceGraph& graph = liveRangeInfo.getInterferenceGraph();

		InterferenceGraph graphCopy(graph);
		DeletedNodes deletedNodes;

		//optimistically remove nodes
		deletNodesFromGraph(graphCopy, deletedNodes, noOfPhysicalRegs);

		// try to color
		// if fail:
		// choose reg with lowest cost to spill
		// add spill code
		allAllocated = assignRegistersToGraph(graph, deletedNodes, startReg,
				noOfPhysicalRegs, spillCount);

		if (allAllocated)
		{
			graph.getMapping(mapping);
			verbosePrint(graph);
			finalizeRegisterAssignment();
			return true;
		}
	} while (!allAllocated && spillCount < noOfSpills);

	return false;
}

void RegisterAllocator::finalizeRegisterAssignment()
{
	for (InstructionsIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		(*iter)->allocateRegs(mapping);
	}
}

void RegisterAllocator::printInstructions(FILE* fptr)
{
	for (InstructionsIter iter = instructions.begin(); iter
			!= instructions.end(); iter++)
	{
		(*iter)->printInstruction(fptr);
	}
}

bool RegisterAllocator::assignRegistersToGraph(InterferenceGraph& graph,
		DeletedNodes& stack, int startReg, int noOfRegs, int &spillCount)
{
	while (stack.size() != 0)
	{
		DeletedNode& deletedNode = stack.top();
		Register assignedRegister = graph.assignRegistersToNode(
				*deletedNode.node, startReg, noOfRegs);

		if (assignedRegister == INVALID_REG)
		{

			RegisterInfo* regToSpillFill = deletedNode.node;
			Instructions modifiedInst;
			spillFillRegister(*regToSpillFill, spillCount, modifiedInst);
			mapping[regToSpillFill->getNo()] = -1 * spillCount;
			spillCount++;
			registerInfo.erase(regToSpillFill->getNo());
			delete regToSpillFill;

			for (InstructionsIter iter = modifiedInst.begin(); iter
					!= modifiedInst.end(); iter++)
			{
				updateRegisterInfo(*(*iter));
			}

			return false;
		}

		stack.pop();
	}
	return true;
}

void RegisterAllocator::spillFillRegister(RegisterInfo& reg, int spillMemory,
		Instructions &modifiedInst)
{
	const RegisterInfo::RegisterUsageSet& useInsts = reg.getUseInstructions();
	instructions.reserve(instructions.size() + useInsts.size());

	const RegisterInfo::RegisterUsageSet& defInsts = reg.getDefInstructions();
	for (RegisterInfo::RegisterUsageSetConstIter iter = defInsts.begin(); iter
			!= defInsts.end(); iter++)
	{
		Instruction& inst = *(*iter);
		Instruction& spillInst = inst.spillInst(reg.getNo(), ++maxReg, -1
				* spillMemory);

		instructions.insert(instructions.begin() + inst.getNo() + 1, &spillInst);

		for (InstructionsIter iter2 = instructions.begin() + inst.getNo() + 2; iter2
				!= instructions.end(); iter2++)
		{
			(*iter2)->setNo((*iter2)->getNo() + 1);
		}

		modifiedInst.push_back(&inst);
		modifiedInst.push_back(&spillInst);
	}

	for (RegisterInfo::RegisterUsageSetConstIter iter = useInsts.begin(); iter
			!= useInsts.end(); iter++)
	{
		Instruction& inst = *(*iter);
		Instruction& fillInst = inst.fillInst(reg.getNo(), ++maxReg, -1
				* spillMemory);

		instructions.insert(instructions.begin() + inst.getNo(), &fillInst);

		for (InstructionsIter iter2 = instructions.begin() + inst.getNo(); iter2
				!= instructions.end(); iter2++)
		{
			(*iter2)->setNo((*iter2)->getNo() + 1);
		}

		modifiedInst.push_back(&inst);
		modifiedInst.push_back(&fillInst);
	}


}

void RegisterAllocator::deletNodesFromGraph(InterferenceGraph& graph,
		DeletedNodes& stack, int noOfPhysicalRegs)
{
	while (graph.getNoNodes() != 0)
	{
		PRINTF("graph size %d\n", graph.getNoNodes());
		graph.print();

		RegisterInfo* node = graph.removeNodeWithDegreeLessThan(noOfPhysicalRegs);
		if (node != NULL)
		{
			PRINTF("register removed %d\n", node->getNo());
			stack.push(DeletedNode(node, false));
		}
		else
		{

			node = graph.removeSpillable();
			PRINTF("register spilled %d with cost %d\n", node->getNo(),
					node->getCost());
			stack.push(DeletedNode(node, true));
		}
	} PRINTF("graph size %d\n", graph.getNoNodes());
	graph.print();
}

