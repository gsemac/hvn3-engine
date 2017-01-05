#ifndef __STRINGABLE_H
#define __STRINGABLE_H
#include <string>

struct IStringable {
	// Returns the text representation of the value of the current object.
	virtual std::string ToString() = 0;
};

#endif