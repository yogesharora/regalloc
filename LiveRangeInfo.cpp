/*
 * LiveRangeInfo.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: Yogesh Arora
 */

#include "globals.h"
#include "LiveRangeInfo.h"

LiveRangeInfo::LiveRangeInfo(Instructions &inst) : instructions(inst)
{
}

LiveRangeInfo::~LiveRangeInfo()
{
}

InterferenceGraph& LiveRangeInfo::getInterferenceGraph()
{
//		// build interference info
//		int instructionSize = maxInstNumber - minInstNumber + 1;
//		RegRanges liveRange(instructionSize);
//		for (int i = 0; i < instructionSize; i++)
//		{
//			liveRange.push_back(RegisterInfoSet());
//		}
//
//		for (RegRangeInfoIter iter = regInfo.begin(); iter != regInfo.end(); iter++)
//		{
//			RegRange &range = iter->second;
//			liveRange[range.start - minInstNumber].insert(range.reg);
//			liveRange[range.end - minInstNumber].insert(range.reg);
//		}
//
//		// build graph
//		for (RegRangeInfoIter iter = regInfo.begin(); iter != regInfo.end(); iter++)
//		{
//			RegRange &range = iter->second;
//			for (int i = range.start + 1; i < range.end; i++)
//			{
//				RegisterInfoSet& interferingRegs = liveRange[i - minInstNumber];
//				for (RegisterInfoSetIter iter2 = interferingRegs.begin(); iter2
//						!= interferingRegs.end(); iter2++)
//				{
//					graph->addInterference(*range.reg, **iter2);
//				}
//			}
//		}
//
//		// create register queues
//		graph->createRegisterQueues();
//	}
//	return *graph;
}

