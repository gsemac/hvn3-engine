#include "UTF8String.h"

std::ostream& operator<< (std::ostream& stream, const Utf8String& str) {

	stream << str.c_str();
	return stream;

}