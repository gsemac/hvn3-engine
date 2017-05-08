#pragma once
#include <utility>

template <typename T>
class ICollection {

public:
	virtual void Add(T& element) = 0;
	virtual void Remove(T& element) = 0;
	virtual void Clear() = 0;
	virtual T& At(size_t index) = 0;
	virtual size_t Count() const = 0;
	virtual bool Contains(T& element) = 0;
	
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