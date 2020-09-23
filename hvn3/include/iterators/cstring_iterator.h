#pragma once

#include <cassert>
#include <iterator>
#include <type_traits>
#include <utility>

namespace hvn3::iterators {

	template<typename ValueT>
	class CStringIterator {

	public:
		using iterator_category = typename std::bidirectional_iterator_tag;
		using value_type = typename ValueT;
		using difference_type = typename std::ptrdiff_t;
		using pointer = typename std::remove_pointer_t<ValueT>*;
		using reference = typename std::remove_pointer_t<ValueT>&;

		CStringIterator() :
			charPointer(nullptr) {
		}
		CStringIterator(ValueT charPointer) :
			charPointer(charPointer) {
		}

		CStringIterator& operator=(const CStringIterator& other) {

			this->charPointer = other.charPointer;

			return *this;

		}

		reference operator*() {

			return Reference();

		}
		const reference operator*() const {

			return Reference();

		}
		pointer operator->() {

			return charPointer;

		}
		reference operator[](difference_type n) {

			return *(*this + n);

		}
		CStringIterator& operator++() {

			Increment();

			return *this;

		}
		CStringIterator operator++(int) {

			ReverseIterator temp = *this;

			Increment();

			return temp;

		}
		CStringIterator& operator+=(difference_type n) {

			Advance(n);

			return *this;

		}
		CStringIterator operator+(difference_type n) {

			ReverseIterator it = *this;

			it += n;

			return it;

		}
		CStringIterator& operator--() {

			Decrement();

			return *this;

		}
		CStringIterator operator--(int) {

			ReverseIterator temp = *this;

			Decrement();

			return temp;

		}
		CStringIterator& operator-=(difference_type n) {

			Advance(-n);

			return *this;

		}
		CStringIterator operator-(difference_type n) {

			ReverseIterator it = *this;

			it -= n;

			return it;

		}

		template<typename ValueT>
		friend bool operator==(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs);
		template<typename ValueT>
		friend bool operator<(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs);
		template<typename ValueT>
		friend bool operator>(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs);
		template<typename ValueT>
		friend typename CStringIterator<ValueT>::difference_type operator-(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs);

	private:
		ValueT charPointer;

		void Increment() {

			assert(*charPointer != '\0');

			++charPointer;

		}
		void Decrement() {

			--charPointer;

		}
		reference Reference() const {

			return *charPointer;

		}
		void Advance(difference_type n) {

			charPointer += n;

		}

	};

	template<typename ValueT>
	bool operator==(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return (lhs.charPointer == rhs.charPointer) ||
			((lhs.charPointer == nullptr || *lhs == '\0') && (rhs.charPointer == nullptr || *rhs == '\0'));

	}
	template<typename ValueT>
	bool operator!=(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return !(lhs == rhs);

	}
	template<typename ValueT>
	bool operator<(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return lhs.charPointer < rhs.charPointer;

	}
	template<typename ValueT>
	bool operator<=(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return lhs < rhs || lhs == rhs;

	}
	template<typename ValueT>
	bool operator>(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return lhs.charPointer > rhs.charPointer;

	}
	template<typename ValueT>
	bool operator>=(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return lhs > rhs || lhs == rhs;

	}
	template<typename ValueT>
	bool operator+(typename CStringIterator<ValueT>::difference_type lhs, const CStringIterator<ValueT>& rhs) {

		return rhs + lhs;

	}
	template<typename ValueT>
	bool operator-(typename CStringIterator<ValueT>::difference_type lhs, const CStringIterator<ValueT>& rhs) {

		return rhs - lhs;

	}
	template<typename ValueT>
	typename CStringIterator<ValueT>::difference_type operator-(const CStringIterator<ValueT>& lhs, const CStringIterator<ValueT>& rhs) {

		return rhs.charPointer - lhs.charPointer;

	}

	template<typename ValueT>
	CStringIterator<ValueT> cstr_begin(ValueT charPointer) {

		assert(charPointer != nullptr);

		static_assert(std::is_pointer_v<ValueT>, "The given value must be a pointer type.");

		return CStringIterator<ValueT>(std::forward<ValueT>(charPointer));

	}
	template<typename ValueT>
	CStringIterator<ValueT> cstr_end(ValueT charPointer) {

		return cstr_begin(charPointer);

	}
	CStringIterator<const char*> cstr_end() {

		return CStringIterator<const char*>();

	}

}