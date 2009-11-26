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
#include <list>

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
	typedef std::list<RegGraphNode*> RemovalQueue;
	RemovalQueue removalQueue;

	struct spillCriteria
	{
		bool operator()(RegGraphNode* a, RegGraphNode* b)
		{
			return a->first->getCost() < b->first->getCost();
		}
	};

	struct removalCriteria
	{
		bool operator()(RegGraphNode* a, RegGraphNode* b)
		{
			return a->second.size() < b->second.size();
		}
	};

	void removeFrontNode();
public:
	InterferenceGraph();
	~InterferenceGraph();
	void addInterference(RegisterInfo &reg1, RegisterInfo &reg2);
	void createRegisterQueues();
	RegisterInfo* removeNodeWithDegreeLessThan(int degree);
	RegisterInfo* removeSpillable();
	void print() const;

	int getNoNodes()
	{
		return graph.size();
	}
};

#endif /* INTERFERENCEGRAPH_H_ */
