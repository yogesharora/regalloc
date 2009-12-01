/*
 * Bitset.cpp
 *
 *  Created on: Oct 8, 2009
 *      Author: yogesh
 */
#include "Bitset.h"
#include "string.h"
#include "stdio.h"
#include "globals.h"

Bitset::Bitset(int len) :
	length(len)
{
	bits = new char[length];
	memset(bits, BIT_OFF, length);
}

Bitset::Bitset(const Bitset &source)
{
	bits = new char[source.length];
	length = source.length;
	memcpy(bits, source.bits, length);
}

Bitset::~Bitset()
{
	delete[] bits;
}

Bitset& Bitset::operator=(const Bitset &lhs)
{
	if (this != &lhs) //avoid self copy
	{
		// assume that number of bits will be the same
		memcpy(bits, lhs.bits, length);
	}
	return *this;
}

Bitset& Bitset::operator&=(const Bitset &lhs)
{
	for (int i = 0; i < length; i++)
	{
		bits[i] &= lhs.bits[i];
	}
	return *this;
}

Bitset& Bitset::operator|=(const Bitset &lhs)
{
	for (int i = 0; i < length; i++)
	{
		bits[i] |= lhs.bits[i];
	}
	return *this;
}

Bitset& Bitset::operator-=(const Bitset &lhs)
{
	for (int i = 0; i < length; i++)
	{
		bits[i] &= ~lhs.bits[i];
	}
	return *this;
}

Bitset Bitset::operator&(const Bitset &lhs)
{
	Bitset res(*this);
	res &= lhs;
	return res;
}

Bitset Bitset::operator|(const Bitset &lhs)
{
	Bitset res(*this);
	res |= lhs;
	return res;
}

Bitset Bitset::operator-(const Bitset &lhs)
{
	Bitset res(*this);
	res -= lhs;
	return res;
}

char& Bitset::operator[](int index)
{
	return bits[index];
}

void Bitset::print(const char* formatString, int start, int end, int min)
{
	for (int i = start; i < end; i++)
	{
		if(bits[i])
		{
			PRINTF(formatString, min+i-start);
		}
	}
}

bool Bitset::operator==(const Bitset &lhs)
{
	for (int i = 0; i < length; i++)
	{
		if (bits[i] != lhs.bits[i])
			return false;
	}
	return true;
}
