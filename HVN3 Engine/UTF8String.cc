#include "UTF8String.h"

Utf8String operator+(const Utf8String& a, const char* b) {

	return Utf8String(std::string(a.c_str()) + b);

}
std::ostream& operator<< (std::ostream& stream, const Utf8String& str) {

	stream << str.c_str();
	return stream;

}
