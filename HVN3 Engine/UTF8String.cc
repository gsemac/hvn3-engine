#include "UTF8String.h"

String operator+(const String& a, const char* b) {

	return String(std::string(a.c_str()) + b);

}
std::ostream& operator<< (std::ostream& stream, const String& str) {

	stream << str.c_str();
	return stream;

}
