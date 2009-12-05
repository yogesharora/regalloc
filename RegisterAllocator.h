/*
 * RegisterAllocator.h
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#ifndef REGISTERALLOCATOR_H_
#define REGISTERALLOCATOR_H_

#include "globals.h"
#include "s3inst.h"
#include "Instruction.h"
#include "RegisterInfo.h"
#include "LiveRangeInfo.h"
#include <vector>
#include <map>
#include <stack>
#include "typedefs.h"

class RegisterAllocator
{
	inst_t instruction;
	int noOfInstructions;
	int noOfRegisters;
	Register maxReg;
	Register originalMaxReg;
	Register minReg;

	Mapping mapping;

	Instructions instructions;

	Registers registerInfo;

	struct DeletedNode
	{
		RegisterInfo* node;
		bool spilled;

		DeletedNode(RegisterInfo* n, bool s) :
			node(n), spilled(s)
		{
		}
	};
	typedef std::stack<DeletedNode> DeletedNodes;

	void initProgramInfo();
	void calcMaxMinRegisters(inst_t instruction);
	void updateRegisterInfo(Instruction&);
	bool isAllocatableRegister(Register no);
	void deletNodesFromGraph(InterferenceGraph& graph, DeletedNodes& stack,
			int noOfPhysicalRegs);
	bool assignRegistersToGraph(InterferenceGraph& graph, DeletedNodes& stack,
			int startReg, int noOfRegs,int &spillCount);
	void spillFillRegister(RegisterInfo& reg, int spillMemory, Instructions &modifiedInst);

	RegisterAllocator(RegisterAllocator&);
    void verbosePrint(InterferenceGraph & graph);
    void finalizeRegisterAssignment();
public:
	RegisterAllocator(inst_t start);
	~RegisterAllocator();
	bool allocateRegs(Register startReg, int noOfPhysicalRegs, int noOfSpills);
	void printInstructions(FILE* fptr);
};

#endif /* REGISTERALLOCATOR_H_ */
