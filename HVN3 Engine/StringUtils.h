#pragma once
#include <vector>
#include <string>
#include <initializer_list>
#include "BitFlags.h"

class String;

namespace StringUtils {

	enum class CharacterCasing {
		Default = 0x00,
		Lower = 0x01,
		Upper = 0x02
	};

	std::vector<std::string> Split(const std::string& str, char delimiter);
	bool IsNullOrEmpty(const String& str);
	bool IsWordBoundary(int ch);
	bool IsNumeric(int ch);

	std::string Trim(const std::string& input_string);
	std::string LTrim(const std::string& input_string);
	std::string RTrim(const std::string& input_string);
	std::string Trim(const std::string& input_string, const std::initializer_list<char>& chars);
	std::string LTrim(const std::string& input_string, const std::initializer_list<char>& chars);
	std::string RTrim(const std::string& input_string, const std::initializer_list<char>& chars);

};