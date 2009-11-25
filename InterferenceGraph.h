/*
 * InterferenceGraph.h
 *
 *  Created on: Nov 25, 2009
 *      Author: Yogesh Arora
 */

#ifndef INTERFERENCEGRAPH_H_
#define INTERFERENCEGRAPH_H_

#include "RegisterInfo.h"
#include <set>
#include <map>
#include <queue>

class InterferenceGraph
{
	typedef std::set<RegisterInfo*> RegNeighbors;
	typedef std::map<int , RegNeighbors> RegGraph;
	RegGraph graph;


//	RemovalQueue removalQueue;
	typedef std::priority_queue<RegisterInfo*> SpillQueue;
	SpillQueue spillQueue;


public:
	InterferenceGraph();
	~InterferenceGraph();
	void addInterference(RegisterInfo &reg1, RegisterInfo &reg2);
	void createRegisterQueues();
};

#endif /* INTERFERENCEGRAPH_H_ */
