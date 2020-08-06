#pragma once

#include <iterator>
#include <memory>
#include <utility>

namespace hvn3::iterators {

	template<typename Iter>
	class ReverseIterator {

	public:
		using iterator_type = Iter;
		using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;
		using value_type = typename std::iterator_traits<iterator_type>::value_type;
		using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
		using pointer = typename std::iterator_traits<iterator_type>::pointer;
		using reference = typename std::iterator_traits<iterator_type>::reference;

		ReverseIterator(const iterator_type& iterator) :
			_base(iterator),
			_temp(iterator) {
		}
		ReverseIterator(const ReverseIterator& other) :
			_base(other._base),
			_temp(other._base) {
		}

		iterator_type base() const {

			return _base;

		}

		ReverseIterator& operator=(const ReverseIterator& other) {

			this->_base = other.base();

			return *this;

		}

		reference operator*() {

			return Reference();

		}
		const reference operator*() const {

			return Reference();

		}
		pointer operator->() {

			return std::addressof(Reference());

		}
		reference operator[](difference_type n) {

			return *(*this + n);

		}
		ReverseIterator& operator++() {

			Increment();

			return *this;

		}
		ReverseIterator operator++(int) {

			ReverseIterator temp = *this;

			Increment();

			return temp;

		}
		ReverseIterator& operator+=(difference_type n) {

			Advance(n);

			return *this;

		}
		ReverseIterator operator+(difference_type n) {

			ReverseIterator it = *this;

			it += n;

			return it;

		}
		ReverseIterator& operator--() {

			Decrement();

			return *this;

		}
		ReverseIterator operator--(int) {

			ReverseIterator temp = *this;

			Decrement();

			return temp;

		}
		ReverseIterator& operator-=(difference_type n) {

			Advance(-n);

			return *this;

		}
		ReverseIterator operator-(difference_type n) {

			ReverseIterator it = *this;

			it -= n;

			return it;

		}

		template<typename Iter>
		friend typename ReverseIterator<Iter>::difference_type operator-(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs);

	private:
		iterator_type _base; // stores the base iterator
		mutable iterator_type _temp; // stores temporary copy of iterator we're dereferencing (for supporting stashing iterators)

		void Increment() {

			--_base;

		}
		void Decrement() {

			++_base;

		}
		reference Reference() const {

			_temp = _base;

			return *--_temp;

		}
		void Advance(difference_type n) {

			_base += -n;

		}

	};

	template<typename Iter>
	bool operator==(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return lhs.base() == rhs.base();

	}
	template<typename Iter>
	bool operator!=(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return !(lhs == rhs);

	}
	template<typename Iter>
	bool operator<(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return lhs.base() < rhs.base();

	}
	template<typename Iter>
	bool operator<=(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return lhs.base() <= rhs.base();

	}
	template<typename Iter>
	bool operator>(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return lhs.base() > rhs.base();

	}
	template<typename Iter>
	bool operator>=(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return lhs.base() >= rhs.base();

	}
	template<typename Iter>
	bool operator+(typename ReverseIterator<Iter>::difference_type lhs, const ReverseIterator<Iter>& rhs) {

		return rhs + lhs;

	}
	template<typename Iter>
	bool operator-(typename ReverseIterator<Iter>::difference_type lhs, const ReverseIterator<Iter>& rhs) {

		return rhs - lhs;

	}
	template<typename Iter>
	typename ReverseIterator<Iter>::difference_type operator-(const ReverseIterator<Iter>& lhs, const ReverseIterator<Iter>& rhs) {

		return rhs._base - lhs._base;

	}

	template<typename Iter>
	ReverseIterator<Iter> make_reverse_iterator(Iter iterator) {

		return ReverseIterator<Iter>(std::forward<Iter>(iterator));

	}

}