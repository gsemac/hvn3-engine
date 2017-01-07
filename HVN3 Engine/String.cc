#include <sstream>
#include <cctype>
#include <algorithm>
#include "String.h"
#include "UTF8String.h"

std::vector<std::string> String::Split(const std::string& str, char delimiter) {

	// Initialize variables.
	std::stringstream ss(str);
	std::string item;
	std::vector<std::string> items;

	// Split the string.
	while (std::getline(ss, item, delimiter))
		items.push_back(item);

	// Return the result.
	return items;

}
std::string String::Trim(const std::string &s) {

	auto wsfront = std::find_if_not(s.begin(), s.end(), [](int c) {return std::isspace(c); });
	auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {return std::isspace(c); }).base();
	return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));

}
bool String::IsNullOrEmpty(const Utf8String& str) {

	return Utf8String::IsNullOrEmpty(str);

}
bool String::IsWordBoundary(int ch) {

	return (ispunct(ch) || isspace(ch)) && !(ch == '\'');

}
bool String::IsNumeric(int ch) {

	return isdigit(ch);

}

std::string Trim(const std::string& input_string) {

	auto front = std::find_if_not(input_string.begin(), input_string.end(), std::isspace);
	auto back = std::find_if_not(input_string.rbegin(), input_string.rend(), std::isspace).base();
	if (front >= back)
		return std::string();

	return std::string(front, back);

}
std::string LTrim(const std::string& input_string) {

	auto front = std::find_if_not(input_string.begin(), input_string.end(), std::isspace);
	return std::string(front, input_string.end());

}
std::string RTrim(const std::string& input_string) {

	auto back = std::find_if_not(input_string.rbegin(), input_string.rend(), std::isspace).base();
	return std::string(input_string.begin(), back);

}
std::string Trim(const std::string& input_string, const std::initializer_list<char>& chars) {

	size_t start = input_string.find_first_not_of(chars);
	size_t end = input_string.find_last_not_of(chars) + 1;
	
	if (start == std::string::npos || end == std::string::npos || start >= end)
		return std::string("");

	return input_string.substr(start, end - start);

}
std::string LTrim(const std::string& input_string, const std::initializer_list<char>& chars) {

	size_t pos = input_string.find_first_not_of(chars);
	if (std::string::npos != pos)
		return input_string.substr(pos);

}
std::string RTrim(const std::string& input_string, const std::initializer_list<char>& chars) {

	size_t pos = input_string.find_last_not_of(chars);
	if (std::string::npos != pos)
		return input_string.substr(0, pos + 1);

}