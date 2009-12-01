/*
 * LivenessAnalysis.h
 *
 *  Created on: Oct 9, 2009
 *      Author: yogesh
 */

#ifndef LIVENESSANALYSIS_H_
#define LIVENESSANALYSIS_H_
#include "s3inst.h"
#include "LivenessInfo.h"
#include "typedefs.h"
#include <map>

struct strCmp
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

class LivenessAnalysis
{
	typedef std::map<char*, int, strCmp> LabelMap;
	LabelMap labelMap;
	int noRegisters;
	int memorySize;
	int minMemOffset;
	int noInstruction;
	LivenessInfo **liveInfo;

	void initProgramInfo();
	void initLivelinessInfo();
	void calculateLiveliness();
	int getHighestReg(inst_t);
	int getR5Offset(inst_t);
	void createUseDefInfo(LivenessInfo &info,  inst_t inst);
	void createSuccessorInfo(LivenessInfo &info,  inst_t inst, int index);
	int memoryToOffset(int mem);
	bool deleteFromList(inst_t cur, inst_t prev);
	void makeNop(inst_t cur);
	void deleteOldLiveInfo();

	const Instructions& instructions;
public:
	LivenessAnalysis(const Instructions& inst);
	~LivenessAnalysis();

	void analyse();
	void print();
};

#endif /* LIVENESSANALYSIS_H_ */
