#pragma once

#include "utility/string_comparison.h"

#include <cctype>
#include <string>

namespace hvn3::utility {

	bool Equals(char char1, char char2, StringComparison comparisonType = StringComparison::Ordinal) {

		switch (comparisonType) {

		case StringComparison::OrdinalIgnoreCase:
			return std::tolower(char1) == std::tolower(char2);

		default:
			return char1 == char2;

		}

	}
	bool Equals(const std::string& str1, const std::string& str2, StringComparison comparisonType = StringComparison::Ordinal) {

		if (str1.length() != str2.length())
			return false;

		for (size_t i = 0; i < str1.size(); ++i)
			if (!Equals(str1[i], str2[i], comparisonType))
				return false;

		return true;

	}

	bool StartsWith(const std::string& str, const std::string& substr, StringComparison comparisonType = StringComparison::Ordinal) {

		if (substr.length() > str.length())
			return false;

		for (size_t i = 0; i < substr.length(); ++i)
			if (!Equals(str[i], substr[i], comparisonType))
				return false;

		return true;

	}
	bool StartsWith(const std::string& str, char ch, StringComparison comparisonType = StringComparison::Ordinal) {

		if (str.length() <= 0)
			return false;

		return Equals(str[0], ch, comparisonType);

	}

}