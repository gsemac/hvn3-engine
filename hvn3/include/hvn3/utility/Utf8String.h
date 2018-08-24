#pragma once
#include "hvn3/utility/Utility.h"
#include "hvn3/fonts/Font.h"
#include "hvn3/allegro/AllegroForwardDeclarations.h"
#include "hvn3/utility/IteratorTemplate.h"
#include <allegro5/utf8.h>
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

	public:
		struct IteratorCharacterAdapter {

			IteratorCharacterAdapter(const String* ustr, int index);
			IteratorCharacterAdapter& operator=(int32_t value);
			operator int32_t() const;

			const String* ustr;
			int index;

		};

		class ReverseIterator;

		class Iterator :
			public IteratorTemplate<Iterator, IteratorCharacterAdapter> {
			friend class base_type;
			friend class ReverseIterator;

		public:
			typedef std::bidirectional_iterator_tag iterator_category;

			Iterator(const String* ustr, int index);

		private:
			IteratorCharacterAdapter _adapter;

			void increment();
			void decrement();
			reference dereference();
			bool equal(const derived_type& rhs) const;
			void advance(difference_type n);		

		};

		class ReverseIterator :
			public IteratorTemplate<ReverseIterator, IteratorCharacterAdapter> {
			friend class base_type;

		public:
			typedef std::bidirectional_iterator_tag iterator_category;

			ReverseIterator(const Iterator& forward_iterator);

		private:
			Iterator _forward_iterator;

			void increment();
			void decrement();
			reference dereference();
			bool equal(const derived_type& rhs) const;
			void advance(difference_type n);

		};

		typedef Iterator iterator;
		typedef Iterator const_iterator;
		typedef ReverseIterator reverse_iterator;
		typedef ReverseIterator const_reverse_iterator;
		typedef size_t size_type;
		typedef int32_t value_type;

		String();
		String(const char* value);
		String(const std::string& value);
		String(const char* value, size_t size);
		String(const String& other);
		String(String&& other);

		iterator begin();
		iterator end();
		const_iterator begin() const;
		const_iterator end() const;
		reverse_iterator rbegin();
		reverse_iterator rend();
		const_reverse_iterator rbegin() const;
		const_reverse_iterator rend() const;

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

		const char* c_str() const;
		
		static bool IsNullOrEmpty(const String& str);

		String& operator=(const char* other);
		String& operator=(const std::string& other);
		String& operator=(const String& other);
		value_type operator[](int index) const;
		String operator+(value_type rhs);

		static const String Empty;
		static const size_type npos;

		size_type size() const;

	private:
		typedef std::shared_ptr<ALLEGRO_USTR> ustr_ptr_t;
		typedef std::unique_ptr<ALLEGRO_USTR, decltype(&al_ustr_free)> ref_ustr_ptr_t;

		mutable ustr_ptr_t _ustr; // pointer to the primary string
		mutable ref_ustr_ptr_t _ref_ustr; // pointer to reference substring (if applicable)
		mutable std::unique_ptr<ALLEGRO_USTR_INFO> _info; // required for storing reference string information; must not be deleted while _ref_ustr is in use

		ALLEGRO_USTR* _getUstrPointer() const;
		ALLEGRO_USTR* _getUstrPointerAndCreateIfNull();
		int _getCodePointByteOffset(size_type index) const;
		void _onWrite() const;

	};

	std::ostream& operator<< (std::ostream& stream, const String& str);
	bool operator==(const String& lhs, const char* rhs);
	bool operator!=(const String& lhs, const char* rhs);
	String operator+(const String& lhs, const char* rhs);
	String operator+(const char* lhs, const String& rhs);

}