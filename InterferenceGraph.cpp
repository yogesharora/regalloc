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
	printf("%d - %d\n", reg1.getNo(), reg2.getNo());
	graph[reg1.getNo()].insert(&reg2);
	graph[reg2.getNo()].insert(&reg1);
}
