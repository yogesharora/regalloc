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
	typedef RegNeighbors::iterator RegNeighborsIter;
	typedef std::map<int , RegNeighbors> RegGraph;
	typedef RegGraph::iterator RegGraphIter;
	typedef RegGraph::const_iterator RegGraphConstIter;
	RegGraph graph;


//	RemovalQueue removalQueue;
	typedef std::priority_queue<RegisterInfo*> SpillQueue;
	SpillQueue spillQueue;


public:
	InterferenceGraph();
	~InterferenceGraph();
	void addInterference(RegisterInfo &reg1, RegisterInfo &reg2);
	void createRegisterQueues();
	void print() const;
};

#endif /* INTERFERENCEGRAPH_H_ */
