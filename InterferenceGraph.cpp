/*
 * InterferenceGraph.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: Yogesh Arora
 */

#include "InterferenceGraph.h"

InterferenceGraph::InterferenceGraph(Registers& reg, Register origMax) :
	origMaxRegister(origMax)
{
	for (RegistersIter regIter = reg.begin(); regIter != reg.end(); regIter++)
	{
		if (isAllocatableRegister(regIter->first))
			graph[regIter->second].neighbors;
	}
}

bool InterferenceGraph::isAllocatableRegister(Register no)
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
InterferenceGraph::~InterferenceGraph()
{
}

void InterferenceGraph::addInterference(RegisterInfo &reg1, RegisterInfo &reg2)
{
	if (isAllocatableRegister(reg1.getNo()) && isAllocatableRegister(
			reg2.getNo()))
	{
		graph[&reg1].neighbors.insert(&reg2);
		graph[&reg2].neighbors.insert(&reg1);
	}
}

void InterferenceGraph::print() const
{
	for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		PRINTF("R%d-cost=%d:", iter->first->getNo(), iter->first->getCost());
		const RegNeighbors &neighbors = iter->second.neighbors;
		for (RegNeighborsIter iter2 = neighbors.begin(); iter2
				!= neighbors.end(); iter2++)
		{
			PRINTF(" R%d", (*iter2)->getNo());
		}
		PRINTF("\n");
	}
}

void InterferenceGraph::getMapping(Mapping& mapping) const
{
	for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		mapping[iter->first->getNo()] = iter->second.assignedReg;
	}
}

RegisterInfo* InterferenceGraph::removeNodeWithDegreeLessThan(int maxDegree)
{
	// traverse the graph ...and remove first node which has highest degree < maxdegree
	int curMaxDegree = -1;
	RegisterInfo* regWithDegLessThanMax = NULL;
	for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		int degree = iter->second.neighbors.size();
		if (degree < maxDegree && degree > curMaxDegree)
		{
			curMaxDegree = degree;
			regWithDegLessThanMax = iter->first;
			if (degree == (maxDegree - 1))
				break;
		}
	}

	removeGraphNode(regWithDegLessThanMax);

	return regWithDegLessThanMax;
}

RegisterInfo* InterferenceGraph::removeSpillable()
{
	// traverse the graph and find first node with minimum cost
	int minCost = -1;
	RegisterInfo* minCostNode = NULL;
	for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		int cost = iter->first->getCost();
		Register regToSpill = iter->first->getNo();
		if (regToSpill <= origMaxRegister)
		{
			if (minCost == -1 || cost < minCost)
			{
				minCost = cost;
				minCostNode = iter->first;
			}
		}
	}

	if (minCostNode == NULL)
	{
		minCost = -1;
		for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
		{
			int cost = iter->first->getCost();
			if (minCost == -1 || cost < minCost)
			{
				minCost = cost;
				minCostNode = iter->first;
			}
		}
	}

	removeGraphNode(minCostNode);
	return minCostNode;
}

void InterferenceGraph::removeGraphNode(RegisterInfo *graphNode)
{
	if (graphNode == NULL)
		return;
	RegNeighbors& neighbors = graph[graphNode].neighbors;
	for (RegNeighborsIter iter = neighbors.begin(); iter != neighbors.end(); iter++)
	{
		RegisterInfo* neighbor = *iter;
		RegNeighbors& neighborsEdges = graph[neighbor].neighbors;
		neighborsEdges.erase(graphNode);
	}

	graph.erase(graphNode);
}

Register InterferenceGraph::assignRegistersToNode(RegisterInfo& reg,
		int startReg, int noOfRegs)
{
	Register assignedReg = INVALID_REG;
	RegNeighbors& neighborsEdges = graph[&reg].neighbors;
	for (int i = startReg; i < startReg + noOfRegs; i++)
	{
		bool canBeUsed = true;
		for (RegNeighborsIter iter = neighborsEdges.begin(); iter
				!= neighborsEdges.end(); iter++)
		{
			RegGraphNodeInfo& neighbor = graph[*iter];
			if (neighbor.assignedReg == i)
			{
				canBeUsed = false;
				break;
			}
		}
		if (canBeUsed)
		{
			assignedReg = i;
			graph[&reg].assignedReg = i;
			break;
		}
	}

	return assignedReg;
}
