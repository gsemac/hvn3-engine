#ifndef __STRING_H
#define __STRING_H
#include <vector>
#include <string>
#include "BitFlags.h"

class Utf8String;

class String {

public:
	static std::vector<std::string> Split(const std::string& str, char delimiter);
	static std::string Trim(const std::string &s);
	static bool IsNullOrEmpty(const Utf8String& str);
	static bool IsWordBoundary(int ch);
	static bool IsNumeric(int ch);

};

enum class CharacterCasing {
	Default = 0x00,
	Lower = 0x01,
	Upper = 0x02
};

#endif