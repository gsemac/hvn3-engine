#pragma once
#include <iostream>
#include <initializer_list>
#include <string>
#include "hvn3/utility/Utility.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/allegro/AllegroForwardDeclarations.h"

namespace hvn3 {

	class String {

	public:
		String();
		String(const char* str);
		String(const std::string& str);
		String(const char* str, size_t size);
		String(ALLEGRO_USTR* ustr);
		String(ALLEGRO_USTR* ustr, ALLEGRO_USTR_INFO* info);
		String(const String& other);
		String(String&& other);
		~String();

		const ALLEGRO_USTR* AlPtr() const;

		String SubString(int end_pos) const;
		String SubString(int start_pos, int end_pos) const;

		String RefSubString(int end_pos) const;
		String RefSubString(int start_pos, int end_pos) const;

		size_t Length() const;
		int Width(const Font& font) const;
		int Height(const Font& font) const;

		int IndexOf(int32_t character) const;
		int IndexOf(int32_t character, int start_pos) const;
		int IndexOf(const char* value) const;
		int LastIndexOf(int32_t character) const;
		int LastIndexOf(int32_t character, int start_pos) const;
		int IndexOfAny(const std::initializer_list<int32_t>& characters, int start_pos) const;
		int IndexOfAny(bool(*f)(int), int start_pos) const;
		int LastIndexOfAny(const std::initializer_list<int32_t>& characters, int start_pos) const;
		int LastIndexOfAny(bool(*f)(int), int start_pos) const;
		int32_t CharAt(int pos) const;

		bool Equals(const char* str);
		bool Contains(const char* value) const;

		void Append(const char* str);
		void Append(int32_t character);
		void Insert(int pos, const char* str);
		void Insert(int pos, int32_t character);
		void Insert(int pos, const String& str);
		void Insert(int pos, const std::string& str);
		void Remove(int pos);
		void Remove(int pos, int length);

		void Clear();
		void Trim();
		void LTrim();
		void RTrim();
		String ToUpper() const;
		String ToLower() const;

		const char* c_str() const;

		static String Empty();
		static bool IsNullOrEmpty(const String& str);

		String& operator=(const char* other);
		String& operator=(const std::string& other);
		String& operator=(const String& other);
		int32_t operator[](const int index);
		friend String operator+(const String& a, const char* b);

	private:
		ALLEGRO_USTR* ustr;
		ALLEGRO_USTR_INFO* info;

		// Returns the index of the codepoint at the given character position. Searches from the closest end of the string.
		int GetCodepointIndex(int char_position) const;
		// Returns the index of the codepoint at the given character position. Searches forwards from the beginning of the string.
		int GetCodepointIndexFromStart(int char_position, int starting_char_position = 0, int starting_codepoint = 0) const;
		// Returns the index of the codepoint at the given character position. Searches backwards from the end of the string.
		int GetCodepointIndexFromEnd(int char_position, int starting_char_position = -1, int starting_codepoint = -1) const;

		void Free();

	};

	std::ostream& operator<< (std::ostream& stream, const String& str);

}