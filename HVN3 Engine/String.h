#ifndef __STRING_H
#define __STRING_H
#include <vector>
#include <string>
#include <initializer_list>
#include "BitFlags.h"

class String;

class StringHelper {

public:
	static std::vector<std::string> Split(const std::string& str, char delimiter);
	static std::string Trim(const std::string &s);
	static bool IsNullOrEmpty(const String& str);
	static bool IsWordBoundary(int ch);
	static bool IsNumeric(int ch);

};

std::string Trim(const std::string& input_string);
std::string LTrim(const std::string& input_string);
std::string RTrim(const std::string& input_string);
std::string Trim(const std::string& input_string, const std::initializer_list<char>& chars);
std::string LTrim(const std::string& input_string, const std::initializer_list<char>& chars);
std::string RTrim(const std::string& input_string, const std::initializer_list<char>& chars);

enum class CharacterCasing {
	Default = 0x00,
	Lower = 0x01,
	Upper = 0x02
};

#endif