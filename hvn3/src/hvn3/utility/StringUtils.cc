#include <sstream>
#include <cctype>
#include <algorithm>
#include "hvn3/utility/StringUtils.h"
#include "hvn3/utility/UTF8String.h"
#include "hvn3/fonts/Font.h"

namespace hvn3 {

	std::vector<std::string> StringUtils::Split(const std::string& str, char delimiter) {

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
	std::vector<std::shared_ptr<String>> StringUtils::Break(const std::shared_ptr<String>& ustr, const Font& font, float width) {

		// Initialize return structure.
		std::vector<std::shared_ptr<String>> resized_lines;

		// Initialize other variables.
		int last_split_index = 0;
		int last_space_index = 0;
		int next_space_index = 0;

		while (1) {
			next_space_index = ustr->IndexOf(' ', last_space_index + 1);
			if (next_space_index == -1) {
				// We've reached the end of the line. Cut off the last word if we need to (and can), then add both lines to
				// the list and exit the loop.
				String tmp(ustr->SubString(last_split_index, (int)ustr->Length()));
				if (tmp.Width(font) > width) {
					resized_lines.push_back(std::make_shared<String>(ustr->SubString(last_split_index, last_space_index + 1)));
					last_split_index = last_space_index + 1;
				}
				resized_lines.push_back(std::make_shared<String>(ustr->SubString(last_split_index, (int)ustr->Length())));
				break;
			}
			else {

				// Check the width of the line up to this space. If it's too long, go back a space and add that (smallest fit).
				String tmp(ustr->SubString(last_split_index, next_space_index));
				if (tmp.Width(font) > width) {
					resized_lines.push_back(std::make_shared<String>(ustr->SubString(last_split_index, last_space_index + 1)));
					last_split_index = last_space_index + 1;
				}
				last_space_index = next_space_index;

			}

		}

		// Return the result.
		return resized_lines;

	}
	bool StringUtils::IsNullOrEmpty(const String& str) {

		return String::IsNullOrEmpty(str);

	}
	bool StringUtils::IsWordBoundary(int ch) {

		return (ispunct(ch) || isspace(ch)) && !(ch == '\'');

	}
	bool StringUtils::IsNumeric(int ch) {

		return !(isdigit(ch) == 0);

	}

	std::string StringUtils::Trim(const std::string& input_string) {

		auto front = std::find_if_not(input_string.begin(), input_string.end(), std::isspace);
		auto back = std::find_if_not(input_string.rbegin(), input_string.rend(), std::isspace).base();
		if (front >= back)
			return std::string();

		return std::string(front, back);

	}
	std::string StringUtils::LTrim(const std::string& input_string) {

		auto front = std::find_if_not(input_string.begin(), input_string.end(), std::isspace);
		return std::string(front, input_string.end());

	}
	std::string StringUtils::RTrim(const std::string& input_string) {

		auto back = std::find_if_not(input_string.rbegin(), input_string.rend(), std::isspace).base();
		return std::string(input_string.begin(), back);

	}
	std::string StringUtils::Trim(const std::string& input_string, const std::initializer_list<char>& chars) {

		size_t start = input_string.find_first_not_of(chars);
		size_t end = input_string.find_last_not_of(chars) + 1;

		if (start == std::string::npos || end == std::string::npos || start >= end)
			return std::string("");

		return input_string.substr(start, end - start);

	}
	std::string StringUtils::LTrim(const std::string& input_string, const std::initializer_list<char>& chars) {

		size_t pos = input_string.find_first_not_of(chars);
		if (std::string::npos != pos)
			return input_string.substr(pos);

	}
	std::string StringUtils::RTrim(const std::string& input_string, const std::initializer_list<char>& chars) {

		size_t pos = input_string.find_last_not_of(chars);
		if (std::string::npos != pos)
			return input_string.substr(0, pos + 1);

	}

}