#include "core/utf8_string.h"

#include "core/algorithm.h"
#include "core/cstring_iterator.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <algorithm>
#include <cassert>

namespace hvn3 {

	// Public members

	const String String::Empty;
	const String::size_type String::npos = -1;

	String::String() :
		_ref_ustr(nullptr, al_ustr_free) {
	}
	String::String(const char* value) :
		String() {

		_ustr = ustr_ptr_t(al_ustr_new(value), al_ustr_free);

	}
	String::String(const std::string& value) :
		String(value.c_str()) {
	}
	String::String(const char* value, size_t size) :
		String() {

		_ustr = ustr_ptr_t(al_ustr_new_from_buffer(value, size), al_ustr_free);

	}
	String::String(const String& other) noexcept :
		String() {

		_ustr = other._ustr;

	}
	String::String(String&& other) noexcept :
		String() {

		_ustr = std::move(other._ustr);
		_ref_ustr = std::move(other._ref_ustr);
		_info = std::move(other._info);

	}

	String String::SubString(int length) const {

		return SubString(0, length);

	}
	String String::SubString(int index, int length) const {

		assert(index >= 0);
		assert(length >= 0);

		if (length == 0)
			return String::Empty;

		// #todo This could be made more efficient since end > begin is a guarantee.
		// Note that we could be creating a substring from a reference substring, so we need to make sure we use the correct pointer.

		int begin = al_ustr_offset(GetUstrPointer(), index);
		int end = al_ustr_offset(GetUstrPointer(), index + length);

		String substr;
		substr._ustr = _ustr;
		substr._info = std::make_unique<ALLEGRO_USTR_INFO>();
		substr._ref_ustr = ref_ustr_ptr_t(const_cast<ALLEGRO_USTR*>(al_ref_ustr(substr._info.get(), GetUstrPointer(), begin, end)), al_ustr_free);

		return substr;

	}

	String::size_type String::Length() const {

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return 0;

		return al_ustr_length(ptr);

	}

	String::size_type String::IndexOf(value_type character) const {

		return IndexOf(character, 0);

	}
	String::size_type String::IndexOf(value_type character, size_type start) const {

		auto index = index_of(begin() + start, end(), character);

		return index == core::npos ?
			npos :
			index;

	}
	String::size_type String::IndexOf(const char* value) const {

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return npos;

		int pos = al_ustr_find_cstr(GetUstrPointer(), 0, value);

		if (pos < 0)
			return npos;

		return static_cast<String::size_type>(pos);

	}
	String::size_type String::LastIndexOf(value_type character) const {

		return LastIndexOf(character, Length());

	}
	String::size_type String::LastIndexOf(value_type character, size_type start) const {

		auto index = last_index_of(begin() + start, end(), character);

		return index == core::npos ?
			npos :
			index;

	}
	String::size_type String::IndexOfAny(std::initializer_list<value_type> characters, size_type start) const {

		auto index = index_of_any(begin() + start, end(), characters.begin(), characters.end());

		return index == core::npos ?
			npos :
			index;

	}
	String::size_type String::IndexOfAny(const std::function<bool(value_type)>& f, size_type start) const {

		auto index = index_of_any(begin() + start, end(), f);

		return index == core::npos ?
			npos :
			index;

	}
	String::size_type String::LastIndexOfAny(std::initializer_list<value_type> characters, size_type start) const {

		auto index = last_index_of_any(begin() + start, end(), characters.begin(), characters.end());

		return index == core::npos ?
			npos :
			index;

	}
	String::size_type String::LastIndexOfAny(const std::function<bool(value_type)>& f, size_type start) const {

		auto index = last_index_of_any(begin() + start, end(), f);

		return index == core::npos ?
			npos :
			index;

	}

	String::value_type String::CharAt(size_type index) const {

		assert(index < Length());

		ALLEGRO_USTR* ptr = GetUstrPointer();

		int pos = al_ustr_offset(ptr, GetByteOffsetFromCodePointOffset(index));

		return al_ustr_get(ptr, pos);

	}

	bool String::Equals(const char* str) const {

		// Note that String instances are allowed to contain the null byte, and may not always be null-terminated.
		// Documentation: https://www.allegro.cc/manual/5/al_cstr
		// Because of this, we can't use C string comparison functions reliably.

		size_t len = strlen(str);

		if (Length() != len)
			return false;

		size_t i = 0;
		for (auto j = begin(); j != end(); ++j, ++i)
			if (*j != str[i])
				return false;

		return true;

	}
	bool String::Equals(const String& value) const {

		return al_ustr_equal(GetUstrPointer(), value.GetUstrPointer());

	}
	bool String::Contains(const char* value) const {

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return false;

		return al_ustr_find_cstr(ptr, 0, value) != -1;

	}
	bool String::StartsWith(const char* value) const {

		return starts_with(begin(), end(), iterators::cstr_begin(value), iterators::cstr_end());

	}
	bool String::EndsWith(const char* value) const {

		return ends_with(begin(), end(), iterators::cstr_begin(value), iterators::cstr_end());

	}

	void String::Append(const char* str) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer(true);

		al_ustr_append_cstr(ptr, str);

	}
	void String::Append(value_type character) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer(true);

		al_ustr_append_chr(ptr, character);

	}
	void String::Append(const String& value) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer(true);
		ALLEGRO_USTR* other_ptr = value.GetUstrPointer();

		if (other_ptr != nullptr)
			al_ustr_append(ptr, other_ptr);

	}
	void String::Insert(size_type index, const char* str) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer(true);
		int pos = GetByteOffsetFromCodePointOffset(index);

		al_ustr_insert_cstr(ptr, pos, str);

	}
	void String::Insert(size_type index, value_type character) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer(true);
		int pos = GetByteOffsetFromCodePointOffset(index);

		al_ustr_insert_chr(ptr, pos, character);

	}
	void String::Insert(size_type index, const String& str) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer(true);
		int pos = GetByteOffsetFromCodePointOffset(index);

		al_ustr_insert(ptr, pos, str.GetUstrPointer());

	}
	void String::Insert(size_type index, const std::string& str) {

		Insert(index, str.c_str());

	}

	void String::RemoveAt(size_type index) {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return;

		int pos = GetByteOffsetFromCodePointOffset(index);

		al_ustr_remove_chr(ptr, pos);

	}
	void String::Remove(size_type index, size_type length) {

		assert(index >= 0);
		assert(index < Length());
		assert(index + length >= 0);
		assert(index + length < Length());

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return;

		int start_pos = GetByteOffsetFromCodePointOffset(index);
		int end_pos = GetByteOffsetFromCodePointOffset(index + length);

		al_ustr_remove_range(ptr, start_pos, end_pos);

	}
	void String::Clear() {

		*this = String();

	}

	void String::Trim() {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return;

		al_ustr_trim_ws(ptr);

	}
	void String::LTrim() {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return;

		al_ustr_ltrim_ws(ptr);

	}
	void String::RTrim() {

		OnWrite(0);

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return;

		al_ustr_rtrim_ws(ptr);

	}
	String String::ToUpper() const {

		String upper = *this;

		for (auto i = upper.begin(); i != upper.end(); ++i)
			*i = towupper(*i);

		return upper;

	}
	String String::ToLower() const {

		String lower = *this;

		for (auto i = lower.begin(); i != lower.end(); ++i)
			*i = towlower(*i);

		return lower;

	}

	bool String::IsNullOrEmpty(const String& str) {

		return (str.GetUstrPointer() == nullptr || str.Length() == 0);

	}

	String& String::operator=(const char* other) {

		*this = String(other);

		return *this;

	}
	String& String::operator=(const std::string& other) {

		*this = String(other);

		return *this;

	}
	String& String::operator=(const String& other) {

		// Share memory with the other object.
		_ustr = other._ustr;

		if (other._ref_ustr) {

			// If the other string is a reference substring, we'll create a reference of the same size.
			_info = std::make_unique<ALLEGRO_USTR_INFO>();
			_ref_ustr = ref_ustr_ptr_t(const_cast<ALLEGRO_USTR*>(al_ref_ustr(_info.get(), other._ref_ustr.get(), 0, al_ustr_size(other._ref_ustr.get()))), al_ustr_free);

		}
		else {

			// Otherwise, clear any existing reference substring information.
			_info.release();
			_ref_ustr.release();

		}

		return *this;

	}
	int32_t String::operator[](int index) const {
		return CharAt(index);
	}
	String String::operator+(value_type rhs) {

		String str(*this);
		str.Append(rhs);

		return str;

	}
	String::operator std::string() const {

		std::string str(begin(), end());

		return str;

	}
	std::ostream& operator<< (std::ostream& stream, const String& str) {

		// Note that if we were to output the c_cstr array for a reference string, it would write the entire string.
		// To keep things simple, we'll write one character at a time so it doesn't matter.

		for (auto i = str.begin(); i != str.end(); ++i)
			stream << static_cast<char>(*i);

		return stream;

	}
	bool operator==(const String& lhs, const char* rhs) {
		return lhs.Equals(rhs);
	}
	bool operator==(const String& lhs, const String& rhs) {
		return lhs.Equals(rhs);
	}
	bool operator!=(const String& lhs, const char* rhs) {
		return !(lhs == rhs);
	}
	String operator+(const String& lhs, const char* rhs) {

		String str = lhs;

		str.Append(rhs);

		return str;

	}
	String operator+(const char* lhs, const String& rhs) {

		String str(lhs);

		str.Append(rhs);

		return str;

	}

	String::size_type String::size() const {

		return Length();

	}
	const char* String::c_str() const {

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (ptr == nullptr)
			return nullptr;

		return al_cstr(ptr);

	}

	// Private members

	ALLEGRO_USTR* String::GetUstrPointer() const {

		ALLEGRO_USTR* ptr = nullptr;

		if (_ref_ustr)
			ptr = _ref_ustr.get();
		else
			ptr = _ustr.get();

		return ptr;

	}
	ALLEGRO_USTR* String::GetUstrPointer(bool createIfNull) {

		ALLEGRO_USTR* ptr = GetUstrPointer();

		if (createIfNull && ptr == nullptr) {

			*this = String("");

			ptr = GetUstrPointer();

		}

		return ptr;

	}
	int String::GetByteOffsetFromCodePointOffset(size_type index) const {

		assert(index >= 0);

		int index_i = static_cast<int>(index);

		assert(index_i >= 0);

		return index_i;

	}
	int32_t String::OnWrite(const int32_t valueWritten) const {

		// Reference substrings can't be written to, so we need to copy.

		if (_ref_ustr) {

			_ustr = ustr_ptr_t(al_ustr_dup(_ref_ustr.get()), al_ustr_free);

			_ref_ustr.release();
			_info.release();

		}
		else if (_ustr && _ustr.use_count() > 1) {

			// We need to copy if any other instances are using the same memory.

			_ustr = ustr_ptr_t(al_ustr_dup(_ustr.get()), al_ustr_free);

		}

		return valueWritten;

	}

}