#include <sstream>
#include <cctype>
#include <cstdint>
#include <algorithm>
#include "hvn3/exceptions/Exception.h"
#include "hvn3/utility/StringUtils.h"
#include "hvn3/utility/UTF8String.h"
#include "hvn3/fonts/Font.h"

namespace hvn3 {
	namespace StringUtils {

		std::vector<std::string> Split(const std::string& str, char delimiter) {

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
		std::vector<std::shared_ptr<String>> Break(const std::shared_ptr<String>& ustr, const Font& font, float width) {

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
		bool IsNullOrEmpty(const String& str) {

			return String::IsNullOrEmpty(str);

		}
		bool IsWordBoundary(int ch) {

			return (ispunct(ch) || isspace(ch)) && !(ch == '\'');

		}
		bool IsNumeric(int ch) {

			return !(isdigit(ch) == 0);

		}
		bool IsAlphanumeric(int ch) {
			return std::isalnum(ch) != 0;
		}
		bool IsHexDigit(int ch) {
			ch = std::tolower(ch);
			return (ch >= 'a' && ch <= 'f') || (ch >= '0' && ch <= '9');
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

			return input_string;

		}
		std::string RTrim(const std::string& input_string, const std::initializer_list<char>& chars) {

			size_t pos = input_string.find_last_not_of(chars);

			if (std::string::npos != pos)
				return input_string.substr(0, pos + 1);

			return input_string;

		}

		size_t SkipWhitespace(const std::string& in, size_t pos) {
			while (pos < in.length() && std::isspace(in[pos]))
				++pos;
			return pos;
		}
		size_t SkipNumeric(const std::string& in, bool allow_decimal, size_t pos) {
			bool seen_decimal = false;
			size_t i = pos;
			while (pos < in.length() && (std::isdigit(in[pos]) || in[pos] == '.')) {
				if (in[pos] == '.') {
					if (!allow_decimal)
						break;
					if (!seen_decimal) // Only one decimal point is allowed.
						seen_decimal = true;
					else
						break;
				}
				++pos;
			}
			if (allow_decimal && pos > i && in[pos - 1] == '.') // Do not accept a single decimal point as a number.
				return --pos;
			return pos;
		}
		size_t SkipHex(const std::string& in, size_t pos) {
			if (in.size() >= 2 && in[0] == '0' && in[1] == 'x')
				pos += 2;
			while (pos < in.size() && IsHexDigit(in[pos]))
				++pos;
			return pos;
		}
		size_t SkipPunctuation(const std::string& in, size_t pos) {
			while (pos < in.length() && std::ispunct(in[pos]))
				++pos;
			return pos;
		}
		size_t SkipIf(const std::string& in, const std::function<bool(int)>& condition, size_t pos) {
			while (pos < in.length() && condition(in[pos]))
				++pos;
			return pos;
		}
		bool NextWord(const std::string& in, std::string& out, size_t& pos) {
			if (in.size() <= 0)
				return false;
			size_t beg = SkipIf(in, [](int c) { return !IsAlphanumeric(c); }, pos);
			bool seen_dash = false;
			pos = SkipIf(in, [&](int c) {
				if (c == '-')
					if (!seen_dash) { // Allow at most one dash to be inside the word.
						seen_dash = true;
						return true;
					}
					else
						return false;
				return IsAlphanumeric(c);
			}, beg);
			if (beg >= pos)
				return false;
			if (in[pos - 1] == '-')
				--pos;
			out = in.substr(beg, pos - beg);
			return true;
		}
		std::string GetNextWord(const std::string& in, size_t pos) {
			std::string out;
			NextWord(in, out, pos);
			return out;
		}
		std::string GetNextIf(const std::string& in, const std::function<bool(int)>& condition, size_t pos) {
			// Skip everything that doesn't meet the condition.
			size_t beg = SkipIf(in, [&](int ch) { return !condition(ch); }, pos);
			// Include everything that does meet the condition.
			pos = SkipIf(in, condition, beg);
			if (beg >= pos)
				return "";
			return in.substr(beg, pos - beg);
		}

		uint64_t ParseHex(const std::string hex) {

			if (hex.length() <= 0)
				throw ArgumentException();

			size_t i = 0;
			bool enable_shorthand = false;

			if (hex.length() >= 2 && hex[0] == '0' && std::tolower(hex[1]) == 'x')
				i += 2;
			else if (hex.length() >= 1 && hex[0] == '#') {
				++i;
				enable_shorthand = true;
			}

			uint64_t out = 0;
			std::stringstream ss;

			ss << std::hex;

			if (hex.length() - i == 3 && enable_shorthand) {
				// Parse as shorthand notation (#ff6600 == #f60).
				ss << hex.substr(i, hex.length() - i);
				uint64_t ss_out = 0;
				ss >> ss_out;
				out |= ((ss_out >> 8) & 0xF) << 20;
				out |= ((ss_out >> 4) & 0xF) << 12;
				out |= ((ss_out >> 0) & 0xF) << 4;
				out |= out >> 4;
				return out;
			}
			else if (hex.length() - i > 0) {
				// Parse as full notation (#ff6600).
				ss << hex.substr(i, hex.length() - i);
				ss >> out;
				return out;
			}
			else
				throw ArgumentException();

		}

		bool IEquals(const std::string& lhs, const std::string& rhs) {
			return Equals(lhs, rhs, true);
		}
		bool Equals(const std::string& lhs, const std::string& rhs, bool ignore_case) {
			if (!ignore_case)
				return lhs == rhs;
			if (lhs.size() != rhs.size())
				return false;
			for (size_t i = 0; i < lhs.size(); ++i)
				if (std::tolower(lhs[i]) != std::tolower(rhs[i]))
					return false;
			return true;
		}
		bool NextSubstringEquals(const std::string& str, size_t pos, const std::string& substr) {

			if (pos + substr.length() >= str.length())
				return false;

			auto i1 = str.begin() + pos;
			auto i2 = substr.begin();

			for (; i2 != substr.end(); ++i1, ++i2)
				if (*i1 != *i2)
					return false;

			return true;

		}

		std::string ToLower(const std::string& input) {

			std::string result;

			result.reserve(input.length());

			std::transform(input.begin(), input.end(), result.begin(), [](const char c) {
				return std::tolower(c);
			});

			return result;

		}
		std::string ToUpper(const std::string& input) {

			std::string result;

			result.reserve(input.length());

			std::transform(input.begin(), input.end(), result.begin(), [](const char c) {
				return std::toupper(c);
			});

			return result;

		}

	}

}