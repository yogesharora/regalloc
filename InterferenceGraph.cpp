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
	graph[&reg1].insert(&reg2);
	graph[&reg2].insert(&reg1);
}

void InterferenceGraph::print() const
{
	for (RegGraphConstIter iter = graph.begin(); iter != graph.end(); iter++)
	{
		printf("R%d:", iter->first->getNo());
		const RegNeighbors &neighbors = iter->second;
		for (RegNeighborsIter iter2 = neighbors.begin(); iter2
				!= neighbors.end(); iter2++)
		{
			printf(" R%d", (*iter2)->getNo());
		}
		printf("\n");
	}
}

void InterferenceGraph::createRegisterQueues()
{
	for(RegGraphIter iter=graph.begin();iter!=graph.end();iter++)
	{
		spillQueue.push(iter->first);
		removalQueue.push(&(*iter));
	}
}
