/*
 * globals.h
 *
 *  Created on: Nov 9, 2009
 *      Author: Yogesh Arora
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stddef.h>
#include <map>
typedef int Register;
#define R0 0
#define R4 4
#define R5 5
#define R6 6
#define R7 7

#include <vector>
typedef std::vector<Register> RegisterSet;
typedef RegisterSet::iterator RegisterSetIter;
typedef RegisterSet::const_iterator RegisterSetConstIter;

struct mappingCompare
{
	bool operator()(const Register a, const Register b)
	{
		return a < b;
	}
};
typedef std::map<Register, int, mappingCompare> Mapping;
typedef Mapping::iterator MappingIter;

#define DEFAULT_K_VALUE 3

#define MEM_MINSIZE 3
#define LIVEOUT_MEM 3
#define REGS_MINSIZE 7
#define INVALID_REG -1
#define NO_REGISTER -1


//#define VERBOSE

#ifdef VERBOSE
#define PRINTF(...) fprintf(stdout, __VA_ARGS__)
#else
#define PRINTF( ...)
#endif

#endif /* GLOBALS_H_ */
