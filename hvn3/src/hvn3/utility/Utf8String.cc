#include "hvn3/utility/UTF8String.h"
#include "hvn3/allegro/AllegroAdapter.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <algorithm>
#include <cassert>

namespace hvn3 {

	// Nested class definitions

	String::IteratorCharacterAdapter::IteratorCharacterAdapter(const String* ustr, int index) : ustr(ustr), index(index) {}
	String::IteratorCharacterAdapter& String::IteratorCharacterAdapter::operator=(int32_t value) {

		assert(ustr != nullptr);
		assert(index >= 0);

		ustr->_onWrite();

		al_ustr_set_chr(ustr->_getUstrPointer(), index, value);

		return *this;

	}
	String::IteratorCharacterAdapter::operator int32_t() const {

		assert(ustr != nullptr);
		assert(index >= 0);

		return al_ustr_get(ustr->_getUstrPointer(), index);

	}

	String::Iterator::Iterator(const String* ustr, int index) :
		_adapter(ustr, index) {
	}
	void String::Iterator::increment() {
		al_ustr_next(_adapter.ustr->_getUstrPointer(), &_adapter.index);
	}
	void String::Iterator::decrement() {
		al_ustr_prev(_adapter.ustr->_getUstrPointer(), &_adapter.index);
	}
	String::Iterator::reference String::Iterator::dereference() {
		return _adapter;
	}
	bool String::Iterator::equal(const derived_type& rhs) const {
		return _adapter.index == rhs._adapter.index;
	}
	void String::Iterator::advance(difference_type n) {

		if (n > 0)
			for (auto i = 0; i < n; ++i)
				increment();
		else if (n < 0)
			for (auto i = 0; i < (std::abs)(n); ++i)
				decrement();

	}

	String::ReverseIterator::ReverseIterator(const Iterator& forward_iterator) :
		_forward_iterator(forward_iterator) {
	}
	void String::ReverseIterator::increment() {

		if (_forward_iterator._adapter.index == 0)
			_forward_iterator = Iterator(_forward_iterator._adapter.ustr, -1);
		else
			_forward_iterator.decrement();

	}
	void String::ReverseIterator::decrement() {		

		_forward_iterator.increment();

	}
	String::ReverseIterator::reference String::ReverseIterator::dereference() {
		return _forward_iterator.dereference();
	}
	bool String::ReverseIterator::equal(const derived_type& rhs) const {
		return _forward_iterator == rhs._forward_iterator;
	}
	void String::ReverseIterator::advance(difference_type n) {
		_forward_iterator.advance(-n);
	}

	// Public methods

	String::String() :
		_ref_ustr(nullptr, al_ustr_free) {
	}
	String::String(const char* value) :
		String() {

		_ustr = ustr_ptr_t(al_ustr_new(value), al_ustr_free);

	}
	String::String(const std::string& value) :
		String(value.c_str()) {}
	String::String(const char* value, size_t size) :
		String() {

		_ustr = ustr_ptr_t(al_ustr_new_from_buffer(value, size), al_ustr_free);

	}
	String::String(const String& other) :
		String() {

		_ustr = other._ustr;

	}
	String::String(String&& other) :
		String() {

		_ustr = std::move(other._ustr);

	}
	String::iterator String::begin() {
		return iterator(this, 0);
	}
	String::iterator String::end() {

		int len = static_cast<int>(Length());

		assert(len > 0);

		return iterator(this, len);

	}
	String::const_iterator String::begin() const {
		return iterator(this, 0);
	}
	String::const_iterator String::end() const {

		int len = static_cast<int>(Length());

		assert(len > 0);

		return iterator(this, len);

	}
	String::reverse_iterator String::rbegin() {
		return reverse_iterator(end() - 1u);
	}
	String::reverse_iterator String::rend() {
		return reverse_iterator(iterator(this, -1));
	}
	String::const_reverse_iterator String::rbegin() const {
		return const_reverse_iterator(end() - 1u);
	}
	String::const_reverse_iterator String::rend() const {
		return const_reverse_iterator(const_iterator(this, -1));
	}
	String String::SubString(int length) const {
		return SubString(0, length);
	}
	String String::SubString(int index, int length) const {

		assert(length >= 0);

		if (length == 0)
			return String::Empty;

		// #todo This could be made more efficient since end > begin is a guarantee.
		// Note that we could be creating a substring from a reference substring, so we need to make sure we use the correct pointer.

		int begin = al_ustr_offset(_getUstrPointer(), index);
		int end = al_ustr_offset(_getUstrPointer(), index + length);

		String substr;
		substr._ustr = _ustr;
		substr._info = std::make_unique<ALLEGRO_USTR_INFO>();
		substr._ref_ustr = ref_ustr_ptr_t(const_cast<ALLEGRO_USTR*>(al_ref_ustr(substr._info.get(), _getUstrPointer(), begin, end)), al_ustr_free);

		return substr;

	}
	String::size_type String::Length() const {

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return 0;

		return al_ustr_length(ptr);

	}
	int String::Width(const Font& font) const {
		return al_get_ustr_width(System::AllegroAdapter::ToFont(font), _getUstrPointer());
	}
	int String::Height(const Font& font) const {

		int bbx, bby, bbw, bbh;
		al_get_ustr_dimensions(System::AllegroAdapter::ToFont(font), _getUstrPointer(), &bbx, &bby, &bbw, &bbh);

		return bby + bbh;

	}
	String::size_type String::IndexOf(value_type character) const {
		return IndexOf(character, 0);
	}
	String::size_type String::IndexOf(value_type character, size_type start) const {

		size_type index = start;

		for (auto i = begin() + start; i != end(); ++i, ++index)
			if (*i == character)
				return index;

		return npos;

	}
	String::size_type String::IndexOf(const char* value) const {

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return npos;

		int pos = al_ustr_find_cstr(_getUstrPointer(), 0, value);

		if (pos < 0)
			return npos;

		return static_cast<String::size_type>(pos);

	}
	String::size_type String::LastIndexOf(value_type character) const {
		return LastIndexOf(character, Length());
	}
	String::size_type String::LastIndexOf(value_type character, size_type start) const {

		size_type index = start;

		for (auto i = rbegin() + start; i != rend(); ++i, --index)
			if (*i == character)
				return index;

		return npos;

	}
	String::size_type String::IndexOfAny(std::initializer_list<value_type> characters, size_type start) const {

		size_type index = start;

		for (auto i = begin() + start; i != end(); ++i, ++index)
			if (hvn3::Contains(characters, static_cast<value_type>(*i)))
				return index;

		return npos;

	}
	String::size_type String::IndexOfAny(const std::function<bool(value_type)>& f, size_type start) const {

		size_type index = start;

		for (auto i = begin() + start; i != end(); ++i, ++index)
			if (f(*i))
				return index;

		return npos;

	}
	String::size_type String::LastIndexOfAny(std::initializer_list<value_type> characters, size_type start) const {

		size_type index = start;

		for (auto i = rbegin() + start; i != rend(); ++i, --index)
			if (hvn3::Contains(characters, static_cast<value_type>(*i)))
				return index;

		return npos;

	}
	String::size_type String::LastIndexOfAny(const std::function<bool(value_type)>& f, size_type start) const {

		size_type index = start;

		for (auto i = rbegin() + start; i != rend(); ++i, --index)
			if (f(*i))
				return index;

		return npos;

	}
	String::value_type String::CharAt(size_type index) const {

		assert(index > Length());

		ALLEGRO_USTR* ptr = _getUstrPointer();

		int pos = al_ustr_offset(ptr, _getCodePointByteOffset(index));

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
	bool String::Contains(const char* value) const {

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return false;

		return al_ustr_find_cstr(ptr, 0, value) != -1;

	}
	void String::Append(const char* str) {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointerAndCreateIfNull();

		al_ustr_append_cstr(ptr, str);

	}
	void String::Append(value_type character) {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointerAndCreateIfNull();

		al_ustr_append_chr(ptr, character);

	}
	void String::Insert(size_type index, const char* str) {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointerAndCreateIfNull();
		int pos = _getCodePointByteOffset(index);

		al_ustr_insert_cstr(ptr, pos, str);

	}
	void String::Insert(size_type index, value_type character) {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointerAndCreateIfNull();
		int pos = _getCodePointByteOffset(index);

		al_ustr_insert_chr(ptr, pos, character);

	}
	void String::Insert(size_type index, const String& str) {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointerAndCreateIfNull();
		int pos = _getCodePointByteOffset(index);

		al_ustr_insert(ptr, pos, str._getUstrPointer());

	}
	void String::Insert(size_type index, const std::string& str) {
		Insert(index, str.c_str());
	}
	void String::RemoveAt(size_type index) {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return;

		int pos = _getCodePointByteOffset(index);

		al_ustr_remove_chr(ptr, pos);

	}
	void String::Remove(size_type index, size_type length) {

		assert(index >= 0);
		assert(index < Length());
		assert(index + length >= 0);
		assert(index + length < Length());

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return;

		int start_pos = _getCodePointByteOffset(index);
		int end_pos = _getCodePointByteOffset(index + length);

		al_ustr_remove_range(ptr, start_pos, end_pos);

	}
	void String::Clear() {
		*this = String();
	}
	void String::Trim() {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return;

		al_ustr_trim_ws(ptr);

	}
	void String::LTrim() {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return;

		al_ustr_ltrim_ws(ptr);

	}
	void String::RTrim() {

		_onWrite();

		ALLEGRO_USTR* ptr = _getUstrPointer();

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
	const char* String::c_str() const {

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			return nullptr;

		return al_cstr(ptr);

	}
	bool String::IsNullOrEmpty(const String& str) {

		return (str._getUstrPointer() == nullptr || str.Length() == 0);

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
	int32_t String::operator[](const int index) {
		return CharAt(index);
	}
	String String::operator+(const char* rhs) {
		return String(std::string(c_str()) + rhs);
	}
	String String::operator+(value_type rhs) {

		String str(*this);
		str.Append(rhs);

		return str;

	}
	std::ostream& operator<< (std::ostream& stream, const String& str) {

		stream << str.c_str();

		return stream;

	}
	bool operator==(const String& lhs, const char* rhs) {
		return lhs.Equals(rhs);
	}
	bool operator!=(const String& lhs, const char* rhs) {
		return !(lhs == rhs);
	}

	const String String::Empty;
	const String::size_type String::npos = -1;

	// Private methods

	ALLEGRO_USTR* String::_getUstrPointer() const {

		if (_ref_ustr)
			return _ref_ustr.get();

		return _ustr.get();

	}
	ALLEGRO_USTR* String::_getUstrPointerAndCreateIfNull() {

		ALLEGRO_USTR* ptr = _getUstrPointer();

		if (ptr == nullptr)
			*this = String("");

		return _getUstrPointer();

	}
	int String::_getCodePointByteOffset(size_type index) const {

		assert(index >= 0);

		int index_i = static_cast<int>(index);

		assert(index_i >= 0);

		return index_i;

	}
	void String::_onWrite() const {

		// Reference substrings can't be written to, so we need to copy.
		if (_ref_ustr) {

			_ustr = ustr_ptr_t(al_ustr_dup(_ref_ustr.get()), al_ustr_free);

			_ref_ustr.release();
			_info.release();

		}
		else

			// We need to copy if any other instances are using the same memory.
			if (_ustr && _ustr.use_count() > 1) {

				_ustr = ustr_ptr_t(al_ustr_dup(_ustr.get()), al_ustr_free);

			}

	}

}