/*
 * RegisterAllocator.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "RegisterAllocator.h"
#include <stack>

using namespace std;

RegisterAllocator::RegisterAllocator(inst_t start) :
	instruction(start), maxReg(INVALID_REG), minReg(INVALID_REG)
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

	for (RegistersIter iter = registerInfo.begin(); iter != registerInfo.end(); iter++)
	{
		if (isAllocatableRegister(iter->second->getNo()))
			liveRangeInfo.addRegister(*(iter->second));
	}
	noOfInstructions = ctr;
	noOfRegisters = (maxReg - minReg) + 1;
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
	instructions.reserve(inst.getNo() + 1);
	instructions[inst.getNo()] = &inst;

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

void RegisterAllocator::allocateRegs(Register startReg, int noOfRegs,
		int noOfSpills)
{
	bool allAllocated = false;
	do
	{
		InterferenceGraph& graph = liveRangeInfo.getInterferenceGraph();

		InterferenceGraph graphCopy(graph);
		DeletedNodes deletedNodes;
		deletNodesFromGraph(graphCopy, deletedNodes, noOfRegs);
		allAllocated = assignRegistersToGraph(graph, deletedNodes, startReg,
				noOfRegs);

		if(allAllocated)
		{
			graph.printAssignedRegisters();
		}
	} while (!allAllocated);

	/* determine live ranges using liveness analysis */
	/* do:
	 calculate cost of spilling
	 build interference graph
	 optimistically remove nodes
	 try to color
	 if fail:
	 choose reg with lowest cost to spill
	 add spill code
	 while (fail)
	 */
}

bool RegisterAllocator::assignRegistersToGraph(InterferenceGraph& graph,
		DeletedNodes& stack, int startReg, int noOfRegs)
{
	while (stack.size() != 0)
	{
		DeletedNode& deletedNode = stack.top();
		Register assignedRegister = graph.assignRegistersToNode(
				*deletedNode.node, startReg, noOfRegs);

		if(assignedRegister==INVALID_REG)
		{
			// spill

			return false;
		}

		stack.pop();
	}
	return true;
}

void RegisterAllocator::deletNodesFromGraph(InterferenceGraph& graph,
		DeletedNodes& stack, int noOfRegs)
{
	while (graph.getNoNodes() != 0)
	{
		printf("graph size %d\n", graph.getNoNodes());
		graph.print();

		RegisterInfo* node = graph.removeNodeWithDegreeLessThan(noOfRegs);
		if (node != NULL)
		{
			printf("register removed %d\n", node->getNo());
			stack.push(DeletedNode(node, false));
		}
		else
		{

			node = graph.removeSpillable();
			printf("register spilled %d with cost %d\n", node->getNo(),
					node->getCost());
			stack.push(DeletedNode(node, true));
		}
	}
	printf("graph size %d\n", graph.getNoNodes());
	graph.print();
}

