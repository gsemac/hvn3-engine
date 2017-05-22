#pragma once
#include <string>

namespace hvn3 {

	class String;

	class Clipboard {

	private:

	public:
		static void SetText(const char* text);
		static void SetText(const std::string& text);
		static void SetText(const String& text);
		static std::string GetText();
		static bool HasText();

	};

}