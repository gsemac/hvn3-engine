#pragma once
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <ios>
#include <memory>
#include <sstream>
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

		uint64_t ParseHex(const std::string hex);

		bool IEquals(const std::string& lhs, const std::string& rhs);
		bool Equals(const std::string& lhs, const std::string& rhs, bool ignore_case = false);
		bool NextSubstringEquals(const std::string& str, size_t pos, const std::string& substr);

		template<typename StringT>
		bool StartsWith(const StringT& input, const char* prefix) {

			size_t len = strlen(prefix);

			if (len > input.size() || len <= 0)
				return false;

			auto i = input.begin();
			size_t j = 0;

			for (; j < len; ++i, ++j) {
				if (*i != *(prefix + j))
					return false;
			}

			return true;

		}
		template<typename StringT1, typename StringT2>
		bool StartsWith(const StringT1& input, const StringT2& prefix) {

			if (prefix.size() > input.size())
				return false;

			auto i = input.begin();
			auto j = prefix.begin();

			for (; i != input.end(); ++i, ++j)
				if (*i != *j)
					return false;

			return true;

		}
		template<typename StringT>
		bool EndsWith(const StringT& input, const char* suffix) {

			size_t len = strlen(suffix);

			if (len > input.size() || len <= 0)
				return false;

			auto i = input.rbegin();
			auto j = suffix + (len - 1);

			for (; j != (suffix - 1); ++i, --j)
				if (*i != *j)
					return false;

			return true;

		}
		template<typename StringT1, typename StringT2>
		bool EndsWith(const StringT1& input, const StringT2& suffix) {

			if (suffix.size() > input.size())
				return false;

			auto i = input.rbegin();
			auto j = suffix.rbegin();

			for (; j != suffix.rend(); ++i, ++j)
				if (*i != *j)
					return false;

			return true;

		}

		// Attempts to construct an object from a string. Uses constructor if available, otherwise uses std::stringstream.
		template <typename T, typename StringT>
		typename std::enable_if<std::is_constructible<T, const std::string&>::value, bool>::type TryParse(const StringT& in, T& out) {

			out = std::move(T(in));

			return true;

		}
		// Attempts to construct an object from a string. Uses constructor if available, otherwise uses std::stringstream.
		template <typename T, typename StringT>
		typename std::enable_if<std::is_arithmetic<T>::value, bool>::type TryParse(const StringT& in, T& out) {

			std::stringstream reader;

			if (in.size() >= 2)
				if (in[0] == '0') {
					if (std::tolower(in[1]) == 'x')
						reader << std::hex;
					else
						reader << std::oct;
				}

			reader << in;
			reader >> out;

			if (reader.fail())
				return false;

			return true;

		}

		template<typename T, typename StringT>
		T Parse(const StringT& in) {

			T out;

			assert(TryParse<T>(in, out));

			return out;

		}

		template <typename T>
		std::string ToString(const T& input) {

			std::stringstream ss;
			
			ss << input;

			return ss.str();

		}

		template<typename T, typename... Args>
		void MakeStringArray(std::vector<std::string>& strings, const T& arg, Args... args) {
			strings.push_back(ToString(arg));
			MakeStringArray(strings, args...);
		}
		template<typename T>
		void MakeStringArray(std::vector<std::string>& strings, const T& arg) {
			strings.push_back(ToString(arg));
		}
		template<typename... Args>
		std::vector<std::string> MakeStringArray(Args... args) {

			std::vector<std::string> strings;

			MakeStringArray(strings, args...);

			return strings;

		}

		template <typename... Args>
		std::string Format(const std::string& format, Args... args) {

			std::vector<std::string> str_args = MakeStringArray(args...);
			std::stringstream out;

			std::string index_str;

			for (auto i = format.begin(); i != format.end(); ++i) {

				if (*i == '{') {

					index_str.clear();

					while (++i != format.end() && std::isdigit(*i))
						index_str.push_back(*i);

					assert(index_str.size() > 0);
					assert(*i == '}');

					int index = std::stoi(index_str);

					assert(index >= 0);
					assert(static_cast<size_t>(index) < str_args.size());

					out << str_args[index];

				}
				else
					out << *i;

			}

			return out.str();

		}

	};

}