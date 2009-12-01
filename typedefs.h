/*
 * typedefs.h
 *
 *  Created on: Nov 30, 2009
 *      Author: Yogesh Arora
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include "Instruction.h"

typedef std::vector<Instruction*> Instructions;
typedef Instructions::iterator InstructionsIter;
typedef Instructions::const_iterator InstructionsConstIter;

#endif /* TYPEDEFS_H_ */
