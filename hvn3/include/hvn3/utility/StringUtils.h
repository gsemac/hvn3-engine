#pragma once
#include <vector>
#include <string>
#include <initializer_list>
#include <memory>
#include "hvn3/utility/BitFlags.h"

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

		std::string Trim(const std::string& input_string);
		std::string LTrim(const std::string& input_string);
		std::string RTrim(const std::string& input_string);
		std::string Trim(const std::string& input_string, const std::initializer_list<char>& chars);
		std::string LTrim(const std::string& input_string, const std::initializer_list<char>& chars);
		std::string RTrim(const std::string& input_string, const std::initializer_list<char>& chars);

	};

}