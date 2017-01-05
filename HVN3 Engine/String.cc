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