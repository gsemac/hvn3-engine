#pragma once

#include "hvn3/allegro/allegro_ustr_iterator.h"
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/iterators/output_transform_iterator.h"
#include "hvn3/utility/Utility.h"

#include <allegro5/utf8.h>

#include <cassert>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <functional>
#include <memory>
#include <string>

namespace hvn3 {

	namespace System {
		class AllegroAdapter;
	}

	class String {

		friend class System::AllegroAdapter;

		int32_t OnWrite(const int32_t valueWritten) const;

	public:
		using iterator = decltype(make_output_transform_iterator(allegro::al_ustr_begin((ALLEGRO_USTR*)nullptr), std::bind1st(std::mem_fun(&String::OnWrite), nullptr)));
		using const_iterator = decltype(make_output_transform_iterator(allegro::al_ustr_cbegin((ALLEGRO_USTR*)nullptr), std::bind1st(std::mem_fun(&String::OnWrite), nullptr)));
		using reverse_iterator = decltype(make_output_transform_iterator(allegro::al_ustr_rbegin((ALLEGRO_USTR*)nullptr), std::bind1st(std::mem_fun(&String::OnWrite), nullptr)));
		using const_reverse_iterator = decltype(make_output_transform_iterator(allegro::al_ustr_crbegin((ALLEGRO_USTR*)nullptr), std::bind1st(std::mem_fun(&String::OnWrite), nullptr)));

		using size_type = size_t;
		using value_type = int32_t;

		static const String Empty;
		static const size_type npos;

		String();
		String(const char* value);
		String(const std::string& value);
		String(const char* value, size_t size);
		String(const String& other) noexcept;
		String(String&& other) noexcept;

		String SubString(int length) const;
		String SubString(int index, int length) const;

		size_type Length() const;
		int Width(const Font& font) const;
		int Height(const Font& font) const;

		size_type IndexOf(value_type character) const;
		size_type IndexOf(value_type character, size_type start) const;
		size_type IndexOf(const char* value) const;
		size_type LastIndexOf(value_type character) const;
		size_type LastIndexOf(value_type character, size_type start) const;
		size_type IndexOfAny(std::initializer_list<value_type> characters, size_type start) const;
		size_type IndexOfAny(const std::function<bool(value_type)>& f, size_type start) const;
		size_type LastIndexOfAny(std::initializer_list<value_type> characters, size_type start) const;
		size_type LastIndexOfAny(const std::function<bool(value_type)>& f, size_type start) const;
		value_type CharAt(size_type index) const;

		bool Equals(const char* value) const;
		bool Equals(const String& value) const;
		bool Contains(const char* value) const;
		bool StartsWith(const char* value) const;
		bool EndsWith(const char* value) const;

		void Append(const char* str);
		void Append(value_type character);
		void Append(const String& value);
		void Insert(size_type index, const char* str);
		void Insert(size_type index, value_type character);
		void Insert(size_type index, const String& str);
		void Insert(size_type index, const std::string& str);
		void RemoveAt(size_type index);
		void Remove(size_type index, size_type length);

		void Clear();
		void Trim();
		void LTrim();
		void RTrim();
		String ToUpper() const;
		String ToLower() const;

		static bool IsNullOrEmpty(const String& str);

		String& operator=(const char* other);
		String& operator=(const std::string& other);
		String& operator=(const String& other);
		value_type operator[](int index) const;
		String operator+(value_type rhs);

		operator std::string() const;

		const_iterator cbegin() const {

			return MakeIterator(allegro::al_ustr_cbegin(GetUstrPointer()));

		}
		const_iterator cend() const {

			return MakeIterator(allegro::al_ustr_cend(GetUstrPointer()));

		}
		iterator begin() {

			return MakeIterator(allegro::al_ustr_begin(GetUstrPointer()));

		}
		iterator end() {

			return MakeIterator(allegro::al_ustr_end(GetUstrPointer()));

		}
		const_iterator begin() const {

			return cbegin();

		}
		const_iterator end() const {

			return cend();

		}
		const_reverse_iterator crbegin() const {

			return MakeIterator(allegro::al_ustr_crbegin(GetUstrPointer()));

		}
		const_reverse_iterator crend() const {

			return MakeIterator(allegro::al_ustr_crend(GetUstrPointer()));

		}
		reverse_iterator rbegin() {

			return MakeIterator(allegro::al_ustr_rbegin(GetUstrPointer()));

		}
		reverse_iterator rend() {

			return MakeIterator(allegro::al_ustr_rend(GetUstrPointer()));

		}
		const_reverse_iterator rbegin() const {

			return crbegin();

		}
		const_reverse_iterator rend() const {

			return crend();

		}

		size_type size() const;

		const char* c_str() const;

	private:
		typedef std::shared_ptr<ALLEGRO_USTR> ustr_ptr_t;
		typedef std::unique_ptr<ALLEGRO_USTR, decltype(&al_ustr_free)> ref_ustr_ptr_t;

		mutable ustr_ptr_t _ustr; // pointer to the primary string
		mutable ref_ustr_ptr_t _ref_ustr; // pointer to reference substring (if applicable)
		mutable std::unique_ptr<ALLEGRO_USTR_INFO> _info; // required for storing reference string information; must not be deleted while _ref_ustr is in use

		ALLEGRO_USTR* GetUstrPointer() const;
		ALLEGRO_USTR* GetUstrPointer(bool createIfNull);
		int GetByteOffsetFromCodePointOffset(size_type index) const;

		template<typename Iter>
		auto MakeIterator(Iter iter) const {

			return make_output_transform_iterator(std::forward<Iter>(iter), std::bind1st(std::mem_fun(&String::OnWrite), this));

		}

	};

	std::ostream& operator<< (std::ostream& stream, const String& str);
	bool operator==(const String& lhs, const char* rhs);
	bool operator==(const String& lhs, const String& rhs);
	bool operator!=(const String& lhs, const char* rhs);
	String operator+(const String& lhs, const char* rhs);
	String operator+(const char* lhs, const String& rhs);

}