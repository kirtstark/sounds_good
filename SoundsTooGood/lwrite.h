#pragma once
#include <fstream>

struct lwrite
{
	unsigned long value;
	unsigned      size;
	lwrite(unsigned long value, unsigned size) :
		value(value), size(size)
	{ }
};

//--------------------------------------------------------------------------
inline std::ostream& operator << (std::ostream& outs, const lwrite& v)
{
	unsigned long value = v.value;
	for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
		outs.put(static_cast <char> (value & 0xFF));
	return outs;
}

