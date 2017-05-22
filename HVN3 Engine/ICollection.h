#pragma once
#include <utility>
#include <cctype>

namespace hvn3 {

	template <typename Iter, typename ConstIter, typename T>
	class ICollection {

	public:
		using iterator = Iter;
		using const_iterator = ConstIter;

		virtual iterator Begin() = 0;
		virtual iterator End() = 0;
		virtual const_iterator Begin() const = 0;
		virtual const_iterator End() const = 0;
		virtual void Add(T& element) = 0;
		virtual void Remove(const T& element) = 0;
		virtual void Clear() = 0;
		virtual T& At(size_t index) = 0;
		virtual size_t Count() const = 0;
		virtual bool Contains(const T& element) = 0;

		virtual T& First() {

			return At(0);

		}
		virtual T& Last() {

			return At(Count() - 1);

		}
		virtual void Reverse() {

			for (size_t i = 0, j = Count(); i < Count() / 2; ++i, --j)
				std::swap(At(i), At(j - 1));

		}

	};

}