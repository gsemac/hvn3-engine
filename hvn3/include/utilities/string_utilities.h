#pragma once

#include "utilities/string_comparison.h"

#include <string>

namespace hvn3::utilities {

	bool Equals(char char1, char char2, StringComparison comparisonType = StringComparison::Ordinal);
	bool Equals(const std::string& str1, const std::string& str2, StringComparison comparisonType = StringComparison::Ordinal);

	bool StartsWith(const std::string& str, const std::string& substr, StringComparison comparisonType = StringComparison::Ordinal);
	bool StartsWith(const std::string& str, char ch, StringComparison comparisonType = StringComparison::Ordinal);

}