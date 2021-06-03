#pragma once

#include <iterator>
#include <type_traits>
#include <utility>

namespace hvn3::core {

	// An iterator that wraps another iterator and applies a unary function to manipulate the return value when dereferenced.
	template<typename Iter, typename UnaryFunction>
	class TransformIterator {

	public:
		using iterator_type = Iter;
		using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;
		using reference = typename std::result_of<const UnaryFunction(typename std::iterator_traits<iterator_type>::reference)>::type;
		using pointer = typename std::add_pointer<reference>::type;
		using value_type = typename std::remove_cv<std::remove_reference<reference>>::type;
		using difference_type = typename std::iterator_traits<iterator_type>::difference_type;

		TransformIterator(const iterator_type& iterator, UnaryFunction transformFunction) :
			_base(iterator),
			_func(transformFunction) {
		}
		TransformIterator(const TransformIterator& other) :
			_base(other._base),
			_func(other._func) {
		}

		iterator_type base() const {

			return _base;

		}

		TransformIterator& operator=(const TransformIterator& other) {

			_base = other.base();
			_func = other._func;

			return *this;

		}

		reference operator*() {

			return Reference();

		}
		reference operator[](difference_type n) {

			return *(*this + n);

		}
		TransformIterator& operator++() {

			Increment();

			return *this;

		}
		TransformIterator operator++(int) {

			TransformIterator temp = *this;

			Increment();

			return temp;

		}
		TransformIterator& operator+=(difference_type n) {

			Advance(n);

			return *this;

		}
		TransformIterator operator+(difference_type n) {

			TransformIterator it = *this;

			it += n;

			return it;

		}
		TransformIterator& operator--() {

			Decrement();

			return *this;

		}
		TransformIterator operator--(int) {

			TransformIterator temp = *this;

			Decrement();

			return temp;

		}
		TransformIterator& operator-=(difference_type n) {

			Advance(-n);

			return *this;

		}
		TransformIterator operator-(difference_type n) {

			TransformIterator it = *this;

			it -= n;

			return it;

		}

	private:
		iterator_type _base;
		UnaryFunction _func;

		void Increment() {

			++_base;

		}
		void Decrement() {

			--_base;

		}
		reference Reference() {

			return _func(*_base);

		}
		void Advance(difference_type n) {

			_base += n;

		}

	};

	template<typename Iter, typename UnaryFunc>
	bool operator==(const TransformIterator<Iter, UnaryFunc>& lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() == rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator!=(const TransformIterator<Iter, UnaryFunc>& lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return !(lhs == rhs);

	}
	template<typename Iter, typename UnaryFunc>
	bool operator<(const TransformIterator<Iter, UnaryFunc>& lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() < rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator<=(const TransformIterator<Iter, UnaryFunc>& lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() <= rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator>(const TransformIterator<Iter, UnaryFunc>& lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() > rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator>=(const TransformIterator<Iter, UnaryFunc>& lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() >= rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator+(typename TransformIterator<Iter, UnaryFunc>::difference_type lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return rhs + lhs;

	}
	template<typename Iter, typename UnaryFunc>
	bool operator-(typename TransformIterator<Iter, UnaryFunc>::difference_type lhs, const TransformIterator<Iter, UnaryFunc>& rhs) {

		return rhs - lhs;

	}

	template<typename Iter, typename UnaryFunction>
	TransformIterator<Iter, UnaryFunction> make_transform_iterator(Iter iterator, UnaryFunction transformFunction) {

		return TransformIterator<Iter, UnaryFunction>(std::forward<Iter>(iterator), std::forward<UnaryFunction>(transformFunction));

	}

}