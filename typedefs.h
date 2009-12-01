/*
 * typedefs.h
 *
 *  Created on: Nov 30, 2009
 *      Author: Yogesh Arora
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <vector>
#include <map>
#include "Instruction.h"
#include "RegisterInfo.h"

typedef std::vector<Instruction*> Instructions;
typedef Instructions::iterator InstructionsIter;
typedef Instructions::const_iterator InstructionsConstIter;

typedef std::map<Register, RegisterInfo*> Registers;
typedef Registers::iterator RegistersIter;
typedef Registers::const_iterator RegistersConstIter;

#endif /* TYPEDEFS_H_ */
