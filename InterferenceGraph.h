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
	struct regInfocompare
	{
		bool operator()(RegisterInfo* a, RegisterInfo* b)
		{
			return a->getNo() < b->getNo();
		}
	};
	typedef std::map<RegisterInfo*, RegNeighbors, regInfocompare> RegGraph;
	typedef RegGraph::value_type RegGraphNode;
	typedef RegGraph::iterator RegGraphIter;
	typedef RegGraph::const_iterator RegGraphConstIter;
	RegGraph graph;

	//	RemovalQueue removalQueue;
	struct spillCriteria
	{
		bool operator()(RegisterInfo* a, RegisterInfo* b)
		{
			return a->getCost() < b->getCost();
		}
	};
	typedef std::priority_queue<RegisterInfo*, std::vector<RegisterInfo*>,
			spillCriteria> SpillQueue;
	SpillQueue spillQueue;

	struct removalCriteria
	{
		bool operator()(RegGraphNode* a, RegGraphNode* b)
		{
			return a->second.size() < b->second.size();
		}
	};
	typedef std::priority_queue<RegGraphNode*, std::vector<RegGraphNode*>,
			removalCriteria> RemovalQueue;
	RemovalQueue removalQueue;

	InterferenceGraph(InterferenceGraph&);
public:
	InterferenceGraph();
	~InterferenceGraph();
	void addInterference(RegisterInfo &reg1, RegisterInfo &reg2);
	void createRegisterQueues();
	void print() const;
};

#endif /* INTERFERENCEGRAPH_H_ */
