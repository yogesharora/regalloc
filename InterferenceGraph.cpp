/*
 * InterferenceGraph.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: Yogesh Arora
 */

#include "InterferenceGraph.h"

InterferenceGraph::InterferenceGraph()
{
}

InterferenceGraph::~InterferenceGraph()
{
}

void InterferenceGraph::addInterference(RegisterInfo &reg1, RegisterInfo &reg2)
{
	graph[&reg1].neighbors.insert(&reg2);
	graph[&reg2].neighbors.insert(&reg1);
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

void InterferenceGraph::createRegisterQueues()
{
	for (RegGraphIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		removalQueue.push_back(&(*iter));
	}
}

RegisterInfo* InterferenceGraph::removeNodeWithDegreeLessThan(int maxDegree)
{
	removalQueue.sort(removalCriteria());
	RegisterInfo& node = *(removalQueue.front()->first);
	int degree = removalQueue.front()->second.neighbors.size();
	if (degree < maxDegree)
	{
		removeFrontNode();
		return &node;
	}
	else
		return NULL;
}

RegisterInfo* InterferenceGraph::removeSpillable()
{
	removalQueue.sort(spillCriteria());
	RegisterInfo& node = *(removalQueue.front()->first);
	removeFrontNode();
	return &node;
}

void InterferenceGraph::removeFrontNode()
{
	RegNeighbors& neighbors = removalQueue.front()->second.neighbors;
	RegisterInfo& nodeToRemove = *(removalQueue.front()->first);
	for (RegNeighborsIter iter = neighbors.begin(); iter != neighbors.end(); iter++)
	{
		RegisterInfo* neighbor = *iter;
		RegNeighbors& neighborsEdges = graph[neighbor].neighbors;
		neighborsEdges.erase(&nodeToRemove);
	}

	graph.erase(&nodeToRemove);
	removalQueue.clear();
	for (RegGraphIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		removalQueue.push_back(&(*iter));
	}
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

void  InterferenceGraph::finalizeRegisterAssignment()
{
	for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		RegisterInfo& regInfo = *(iter->first);
		Register assignedReg = iter->second.assignedReg;

		regInfo.assignRegisterToInstructions(assignedReg);
	}
}
