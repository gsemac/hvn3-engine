#include "UTF8String.h"
#include "FrameworkAdapter.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

namespace hvn3 {

	// Public methods

	String::String() {

		ustr = nullptr;
		info = nullptr;

	}
	String::String(const char* str) {

		ustr = al_ustr_new(str);
		info = nullptr;

	}
	String::String(const std::string& str) {

		ustr = al_ustr_new(str.c_str());
		info = nullptr;

	}
	String::String(const char* str, size_t size) {

		ustr = al_ustr_new_from_buffer(str, size);
		info = nullptr;

	}
	String::String(ALLEGRO_USTR* ustr) :
		String(ustr, nullptr) {
	}
	String::String(ALLEGRO_USTR* ustr, ALLEGRO_USTR_INFO* info) :
		ustr(ustr), info(info) {
	}
	String::String(const String& other) {

		// Free existing string, if needed.
		Free();

		// Copy the other string (do not use same memory).
		ustr = al_ustr_dup(other.ustr);

	}
	String::String(String&& other) {

		// Free existing string if needed.
		Free();

		// Copy a pointer to the other object's ustr.
		ustr = other.ustr;
		info = other.info;

		// Nullify the other ustr.
		other.ustr = nullptr;
		other.info = nullptr;

	}
	String::~String() {

		if (ustr) al_ustr_free(ustr);
		ustr = nullptr;

		if (info) delete info;
		info = nullptr;

	}

	const ALLEGRO_USTR* String::AlPtr() const {

		return ustr;

	}

	String String::SubString(int end_pos) const {

		return SubString(0, end_pos);

	}
	String String::SubString(int start_pos, int end_pos) const {

		// So that we can treat this like a regular string, find the nth and mth code points 
		// based on the provided positions.

		int i = 0, it = 0;

		for (i = 0; i < start_pos; i++)
			al_ustr_next(ustr, &it);
		start_pos = it;

		for (; i < end_pos; i++)
			al_ustr_next(ustr, &it);
		end_pos = it;

		return String(al_ustr_dup_substr(ustr, start_pos, end_pos));

	}

	String String::RefSubString(int end_pos) const {

		return RefSubString(0, end_pos);

	}
	String String::RefSubString(int start_pos, int end_pos) const {

		// Get the start and end positions of the substring.
		int start = GetCodepointIndex(start_pos);
		int end = GetCodepointIndexFromStart(end_pos, start_pos, start);

		ALLEGRO_USTR_INFO* inf = new ALLEGRO_USTR_INFO;
		return String((ALLEGRO_USTR*)al_ref_ustr(inf, ustr, start, end), inf);

	}

	size_t String::Length() const {

		return (ustr) ? al_ustr_length(ustr) : 0;

	}
	int String::Width(const Font& in_font) const {

		return Width(&in_font);

	}
	int String::Width(const Font* in_font) const {

		return al_get_ustr_width(System::FrameworkAdapter::FromFont(*in_font), ustr);

	}
	int String::Height(const Font& int_font) const {

		return Height(&int_font);

	}
	int String::Height(const Font* int_font) const {

		int bbx, bby, bbw, bbh;
		al_get_ustr_dimensions(System::FrameworkAdapter::FromFont(*int_font), ustr, &bbx, &bby, &bbw, &bbh);
		return bby + bbh;

	}

	int String::IndexOf(int32_t character) const {

		return IndexOf(character, 0);

	}
	int String::IndexOf(int32_t character, int start_pos) const {

		// Find the code point corresponding to the starting position.
		start_pos = GetCodepointIndex(start_pos);

		// Hop from code point to code point until we find the character we want.
		int index = start_pos;
		for (int i = start_pos; i < (int)al_ustr_size(ustr);) {
			int32_t cp = al_ustr_get_next(ustr, &i);
			if (cp == character) return index;
			++index;
		}

		// The character was not found.
		return -1;

	}
	int String::IndexOf(const char* value) const {

		if (!ustr)
			return -1;

		return al_ustr_find_cstr(ustr, 0, value);

	}
	int String::LastIndexOf(int32_t character) const {

		return LastIndexOf(character, (int)Length());

	}
	int String::LastIndexOf(int32_t character, int start_pos) const {

		// Find the code point corresponding to the starting position.
		start_pos = GetCodepointIndex(start_pos);

		// Hop from code point to code point until we find the character we want.
		int index = start_pos;
		for (int i = start_pos; i > 0;) {
			int32_t cp = al_ustr_prev_get(ustr, &i);
			if (cp == character) return index - 1;
			--index;
		}

		// The character was not found.
		return -1;

	}
	int String::IndexOfAny(const std::initializer_list<int32_t>& characters, int start_pos) const {

		// Find the code point corresponding to the starting position.
		start_pos = GetCodepointIndex(start_pos);

		// Hop from code point to code point until we find the character we want.
		int index = start_pos;
		for (int i = start_pos; i < (int)al_ustr_size(ustr);) {
			int32_t cp = al_ustr_get_next(ustr, &i);
			if (hvn3::Contains(characters, cp)) return index;
			++index;
		}

		// The character was not found.
		return -1;

	}
	int String::IndexOfAny(bool(*f)(int), int start_pos) const {

		// Find the code point corresponding to the starting position.
		start_pos = GetCodepointIndex(start_pos);

		// Hop from code point to code point until we find the character we want.
		int index = start_pos;
		for (int i = start_pos; i < (int)al_ustr_size(ustr);) {
			int32_t cp = al_ustr_get_next(ustr, &i);
			if (f(cp)) return index;
			++index;
		}

		// The character was not found.
		return -1;

	}
	int String::LastIndexOfAny(const std::initializer_list<int32_t>& characters, int start_pos) const {

		// Find the code point corresponding to the starting position.
		start_pos = GetCodepointIndex(start_pos);

		// Hop from code point to code point until we find the character we want.
		int index = start_pos;
		for (int i = start_pos; i > 0;) {
			int32_t cp = al_ustr_prev_get(ustr, &i);
			if (hvn3::Contains(characters, cp)) return index - 1;
			--index;
		}

		// The character was not found.
		return -1;

	}
	int String::LastIndexOfAny(bool(*f)(int), int start_pos) const {

		// Find the code point corresponding to the starting position.
		start_pos = GetCodepointIndex(start_pos);

		// Hop from code point to code point until we find the character we want.
		int index = start_pos;
		for (int i = start_pos; i > 0;) {
			int32_t cp = al_ustr_prev_get(ustr, &i);
			if (f(cp)) return index - 1;
			--index;
		}

		// The character was not found.
		return -1;

	}
	int32_t String::CharAt(int pos) const {

		// Find the code point corresponding to this index.
		pos = GetCodepointIndex(pos);

		// Return the character at this position.
		return al_ustr_get(ustr, pos);

	}

	bool String::Equals(const char* str) {

		ALLEGRO_USTR* compare_to = al_ustr_new(str);
		if (!compare_to) return false;
		bool result = al_ustr_equal(ustr, compare_to);
		al_ustr_free(compare_to);
		return result;

	}
	bool String::Contains(const char* value) const {

		// Return false if the string is null.
		if (!ustr)
			return false;


		return al_ustr_find_cstr(ustr, 0, value) != -1;

	}

	void String::Append(const char* str) {

		if (!ustr)
			ustr = al_ustr_new(str);
		else
			al_ustr_append_cstr(ustr, str);

	}
	void String::Append(int32_t character) {

		if (!ustr) ustr = al_ustr_new("");
		al_ustr_append_chr(ustr, character);

	}
	void String::Insert(int pos, const char* str) {

		pos = GetCodepointIndex(pos);
		al_ustr_insert_cstr(ustr, pos, str);

	}
	void String::Insert(int pos, int32_t character) {

		pos = GetCodepointIndex(pos);
		al_ustr_insert_chr(ustr, pos, character);

	}
	void String::Insert(int pos, const String& str) {

		pos = GetCodepointIndex(pos);
		al_ustr_insert(ustr, pos, str.ustr);

	}
	void String::Insert(int pos, const std::string& str) {

		pos = GetCodepointIndex(pos);
		al_ustr_insert_cstr(ustr, pos, str.c_str());

	}
	void String::Remove(int pos) {

		if (pos < 0) return;
		pos = GetCodepointIndex(pos);
		al_ustr_remove_chr(ustr, pos);

	}
	void String::Remove(int pos, int length) {
		if (!ustr) return;
		if (pos < 0 || length < 0) return;

		int start_pos = GetCodepointIndex(pos);
		int end_pos = GetCodepointIndexFromStart(pos + length, pos, start_pos);

		if (start_pos < 0 || end_pos < 0)
			return;

		al_ustr_remove_range(ustr, start_pos, end_pos);

	}

	void String::Clear() {

		Free();

		ustr = al_ustr_new("");

	}
	void String::Trim() {

		al_ustr_trim_ws(ustr);

	}
	void String::LTrim() {

		al_ustr_ltrim_ws(ustr);

	}
	void String::RTrim() {

		al_ustr_rtrim_ws(ustr);

	}
	String String::ToUpper() const {

		String upper = *this;

		// Convert all charcters to uppercase.
		// Note that "i" will always be positive, but we need to use an integer to pass it into the al_ustr functions.
		for (int i = 0, pos = 0; static_cast<size_t>(i) < al_ustr_size(upper.ustr);) {
			int32_t c = al_ustr_get(upper.ustr, i);
			al_ustr_set_chr(upper.ustr, pos, towupper(c));
			al_ustr_next(upper.ustr, &i);
			++pos;
		}

		return upper;

	}
	String String::ToLower() const {

		String lower = *this;

		// Convert all charcters to uppercase.
		// Note that "i" will always be positive, but we need to use an integer to pass it into the al_ustr functions.
		for (int i = 0, pos = 0; static_cast<size_t>(i) < al_ustr_size(lower.ustr);) {
			int32_t c = al_ustr_get(lower.ustr, i);
			al_ustr_set_chr(lower.ustr, pos, towlower(c));
			al_ustr_next(lower.ustr, &i);
			++pos;
		}

		return lower;

	}

	const char* String::c_str() const {

		if (ustr)
			return al_cstr(ustr);
		return nullptr;

	}

	String String::Empty() {

		return String("");

	}
	bool String::IsNullOrEmpty(const String& str) {

		return (!str.ustr || str.Length() == 0);

	}

	String& String::operator=(const char* other) {

		Free();

		ustr = al_ustr_new(other);

		return *this;

	}
	String& String::operator=(const std::string& other) {

		Free();

		ustr = al_ustr_new(other.c_str());

		return *this;

	}
	String& String::operator=(const String& other) {

		Free();

		ustr = al_ustr_dup(other.ustr);

		return *this;

	}
	int32_t String::operator[](const int index) {

		return CharAt(index);

	}
	String operator+(const String& a, const char* b) {

		return String(std::string(a.c_str()) + b);

	}
	std::ostream& operator<< (std::ostream& stream, const String& str) {

		stream << str.c_str();
		return stream;

	}

	// Private methods

	int String::GetCodepointIndex(int char_position) const {

		if (char_position < (int)Length() / 2)
			return GetCodepointIndexFromStart(char_position);
		else
			return GetCodepointIndexFromEnd(char_position);

	}
	int String::GetCodepointIndexFromStart(int char_position, int starting_char_position, int starting_codepoint) const {

		// Search from the beginning.
		int it = starting_codepoint;
		for (int i = starting_char_position; i < char_position; i++)
			al_ustr_next(ustr, &it);
		return it;

	}
	int String::GetCodepointIndexFromEnd(int char_position, int starting_char_position, int starting_codepoint) const {

		// Search from the end.
		int it = (starting_codepoint < 0) ? (int)al_ustr_size(ustr) : starting_codepoint;
		for (int i = (starting_char_position < 0) ? (int)Length() : starting_char_position; i > char_position; --i)
			al_ustr_prev(ustr, &it);
		return it;

	}
	void String::Free() {

		if (ustr)
			al_ustr_free(ustr);

		if (info)
			delete info;

		ustr = nullptr;
		info = nullptr;

	}

}