#pragma once

#include <iterator>
#include <utility>

namespace hvn3 {

	// An iterator that wraps another iterator and applies a unary function to proxy values assigned to it.
	template<typename Iter, typename UnaryFunction>
	class OutputTransformIterator {

	public:
		using iterator_type = Iter;

		class Proxy {

			friend class OutputTransformIterator;

			using reference = typename std::iterator_traits<iterator_type>::reference;

		public:
			Proxy(const Proxy& other) :
				base(other.base),
				func(other.func) {
			}

			reference operator*() {

				return *base;

			}
			operator reference() {

				return *base;

			}

			template<typename CastToType>
			operator CastToType() {

				return static_cast<CastToType>(*base);

			}

			template<typename T>
			Proxy& operator=(T rhs) {

				*base = func(rhs);

				return *this;

			}
			Proxy& operator=(const Proxy& rhs) {

				base = rhs.base;
				func = rhs.func;

				return *this;

			}

			template<typename CompareToType>
			bool operator==(CompareToType other) const {

				return *base == other;

			}
			template<typename CompareToType>
			bool operator!=(CompareToType other) const {

				return *base != other;

			}

			friend std::ostream& operator<<(std::ostream& lhs, const Proxy& rhs) {

				lhs << *rhs.base;

				return lhs;

			}

		private:
			Proxy(const iterator_type& base, UnaryFunction func) :
				base(base),
				func(func) {
			}

			iterator_type base;
			UnaryFunction func;

		};

		using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;
		using value_type = Proxy;
		using difference_type = typename std::iterator_traits<iterator_type>::difference_type;
		using pointer = Proxy*;
		using reference = Proxy&;

		OutputTransformIterator(const iterator_type& iterator, UnaryFunction transformFunction) :
			_proxy(iterator, transformFunction) {
		}
		OutputTransformIterator(const OutputTransformIterator& other) :
			_proxy(other._proxy) {
		}

		iterator_type base() const {

			return _proxy.base;

		}

		OutputTransformIterator& operator=(const OutputTransformIterator& other) {

			_proxy = other._proxy;

			return *this;

		}

		reference operator*() {

			return Reference();

		}
		reference operator[](difference_type n) {

			return *(*this + n);

		}
		OutputTransformIterator& operator++() {

			Increment();

			return *this;

		}
		OutputTransformIterator operator++(int) {

			OutputTransformIterator temp = *this;

			Increment();

			return temp;

		}
		OutputTransformIterator& operator+=(difference_type n) {

			Advance(n);

			return *this;

		}
		OutputTransformIterator operator+(difference_type n) {

			OutputTransformIterator it = *this;

			it += n;

			return it;

		}
		OutputTransformIterator& operator--() {

			Decrement();

			return *this;

		}
		OutputTransformIterator operator--(int) {

			OutputTransformIterator temp = *this;

			Decrement();

			return temp;

		}
		OutputTransformIterator& operator-=(difference_type n) {

			Advance(-n);

			return *this;

		}
		OutputTransformIterator operator-(difference_type n) {

			OutputTransformIterator it = *this;

			it -= n;

			return it;

		}

	private:
		Proxy _proxy;

		void Increment() {

			++_proxy.base;

		}
		void Decrement() {

			--_proxy.base;

		}
		reference Reference() {

			return _proxy;

		}
		void Advance(difference_type n) {

			_proxy.base += n;

		}

	};

	template<typename Iter, typename UnaryFunc>
	bool operator==(const OutputTransformIterator<Iter, UnaryFunc>& lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() == rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator!=(const OutputTransformIterator<Iter, UnaryFunc>& lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return !(lhs == rhs);

	}
	template<typename Iter, typename UnaryFunc>
	bool operator<(const OutputTransformIterator<Iter, UnaryFunc>& lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() < rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator<=(const OutputTransformIterator<Iter, UnaryFunc>& lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() <= rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator>(const OutputTransformIterator<Iter, UnaryFunc>& lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() > rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator>=(const OutputTransformIterator<Iter, UnaryFunc>& lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return lhs.base() >= rhs.base();

	}
	template<typename Iter, typename UnaryFunc>
	bool operator+(typename OutputTransformIterator<Iter, UnaryFunc>::difference_type lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return rhs + lhs;

	}
	template<typename Iter, typename UnaryFunc>
	bool operator-(typename OutputTransformIterator<Iter, UnaryFunc>::difference_type lhs, const OutputTransformIterator<Iter, UnaryFunc>& rhs) {

		return rhs - lhs;

	}

	template<typename Iter, typename UnaryFunction>
	OutputTransformIterator<Iter, UnaryFunction> make_output_transform_iterator(Iter iterator, UnaryFunction transformFunction) {

		return OutputTransformIterator<Iter, UnaryFunction>(std::forward<Iter>(iterator), std::forward<UnaryFunction>(transformFunction));

	}

}