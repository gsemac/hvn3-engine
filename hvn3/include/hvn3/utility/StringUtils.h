#pragma once
#include <cctype>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <ios>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace hvn3 {

	class String;
	class Font;

	namespace StringUtils {

		enum class CharacterCasing {
			Default = 0x00,
			Lower = 0x01,
			Upper = 0x02
		};

		std::vector<std::string> Split(const std::string& str, char delimiter);
		std::vector<std::shared_ptr<String>> Break(const std::shared_ptr<String>& ustr, const Font& font, float width);
		bool IsNullOrEmpty(const String& str);
		bool IsWordBoundary(int ch);
		bool IsNumeric(int ch);
		bool IsAlphanumeric(int ch);
		bool IsHexDigit(int ch);

		bool StartsWith(const std::string& str, const std::string& prefix) {
			if (prefix.length() > str.length())
				return false;
			for (size_t i = 0; i < prefix.length(); ++i)
				if (str[i] != prefix[i])
					return false;
			return true;
		}

		std::string Trim(const std::string& input_string);
		std::string LTrim(const std::string& input_string);
		std::string RTrim(const std::string& input_string);
		std::string Trim(const std::string& input_string, const std::initializer_list<char>& chars);
		std::string LTrim(const std::string& input_string, const std::initializer_list<char>& chars);
		std::string RTrim(const std::string& input_string, const std::initializer_list<char>& chars);

		// Returns the position of the first non-whitespace character.
		size_t SkipWhitespace(const std::string& in, size_t pos = 0);
		// Returns the position of the first non-numeric character.
		size_t SkipNumeric(const std::string& in, bool allow_decimal = false, size_t pos = 0);
		// Returns the position of the first non-hexadecimal character.
		size_t SkipHex(const std::string& in, size_t pos = 0);
		// Returns the position of the first non-punctuation character.
		size_t SkipPunctuation(const std::string& in, size_t pos = 0);
		// Skips all characters that match the given condition.
		size_t SkipIf(const std::string& in, const std::function<bool(int)>& condition, size_t pos = 0);
		// Reads the next word from the string and stores it in out. Returns true if a word was read; false otherwise.
		bool NextWord(const std::string& in, std::string& out, size_t& pos);
		std::string GetNextWord(const std::string& in, size_t pos);
		std::string GetNextIf(const std::string& in, const std::function<bool(int)>& condition, size_t pos);

		unsigned long ParseHex(const std::string hex);

		bool IEquals(const std::string& lhs, const std::string& rhs);
		bool Equals(const std::string& lhs, const std::string& rhs, bool ignore_case = false);

		// Attempts to construct an object from a string. Uses constructor if available, otherwise uses std::stringstream.
		template <typename T>
		typename std::enable_if<std::is_constructible<T, const std::string&>::value, T>::type ConvertFromString(const std::string& in) {
			return T(in);
		}
		// Attempts to construct an object from a string. Uses constructor if available, otherwise uses std::stringstream.
		template <typename T>
		typename std::enable_if<std::is_arithmetic<T>::value, T>::type ConvertFromString(const std::string& in) {
			std::stringstream reader;
			if (in.size() >= 2)
				if (in[0] == '0') {
					if (std::tolower(in[1]) == 'x')
						reader << std::hex;
					else
						reader << std::oct;
				}
			reader << in;
			T out;
			reader >> out;
			return out;
		}

	};

}