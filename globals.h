/*
 * globals.h
 *
 *  Created on: Nov 9, 2009
 *      Author: Yogesh Arora
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stddef.h>
#define INVALID_REG -1
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
#define DEFAULT_K_VALUE 3

//#define VERBOSE

#ifdef VERBOSE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF( ...)
#endif

#endif /* GLOBALS_H_ */
