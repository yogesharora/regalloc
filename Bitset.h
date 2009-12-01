/*
 * Bitset
 *
 *  Created on: Oct 8, 2009
 *      Author: yogesh
 */

#ifndef BITSET_H
#define BITSET_H

#define BIT_ON 1
#define BIT_OFF 0

class Bitset
{
	char* bits;
	int length;

public:
	Bitset(int length);
	Bitset(const Bitset &bit);
	~Bitset();
	Bitset& operator=(const Bitset &lhs);
	Bitset& operator&=(const Bitset &lhs);
	Bitset& operator|=(const Bitset &lhs);
	Bitset& operator-=(const Bitset &lhs);
	Bitset operator&(const Bitset &lhs);
	Bitset operator|(const Bitset &lhs);
	Bitset operator-(const Bitset &lhs);
	bool operator==(const Bitset &lhs);
	char& operator[](int index);
	void print(const char* prefix, int start, int end, int min=0);
};

#endif /* BITSET_ */
