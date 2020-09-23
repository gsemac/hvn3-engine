#pragma once

#include "math/math.h"
#include "math/quadrant.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <numeric>
#include <type_traits>

namespace hvn3::math {

	template<typename T, size_t Size>
	class VectorBase {

		typedef size_t index_t;
		typedef std::array<T, Size> container_t;

	public:
		typedef size_t size_t;
		typedef typename container_t::iterator iterator;
		typedef typename container_t::const_iterator const_iterator;
		typedef typename container_t::reverse_iterator reverse_iterator;
		typedef typename container_t::const_reverse_iterator const_reverse_iterator;

		VectorBase() :
			VectorBase({}) {
		}
		VectorBase(std::initializer_list<T> elements) {

			assert(elements.size() <= Size);

			std::fill(this->elements.begin(), this->elements.end(), static_cast<T>(0));
			std::copy(elements.begin(), elements.end(), this->elements.begin());

		}
		template<typename U>
		VectorBase(const VectorBase<U, Size>& other) :
			VectorBase() {

			*this = other;

		}
		template<typename U>
		VectorBase(VectorBase<U, Size>&& other) :
			VectorBase() {

			elements = std::move(other.elements);

		}

		template<typename U>
		T DotProduct(const VectorBase<U, Size>& other) const {

			T dotProduct = static_cast<T>(0);

			for (size_t i = 0; i < size(); ++i)
				dotProduct += (*this)[i] * static_cast<T>(other[i]);

			return dotProduct;

		}

		T Length() const {

			return std::sqrt(SquaredLength());

		}
		T SquaredLength() const {

			T lengthSquared = std::accumulate(elements.begin(), elements.end(), static_cast<T>(0),
				[](auto sum, auto n) -> T {
					return sum + std::pow(n, static_cast<T>(2));
				});

			return lengthSquared;

		}

		VectorBase<T, Size> Normalize() const {

			return *this / Length();

		}

		size_t size() const {

			return Size;

		}
		iterator begin() {

			return elements.begin();

		}
		iterator end() {

			return elements.end();

		}
		const_iterator begin() const {

			return cbegin();

		}
		const_iterator end() const {

			return cend();

		}
		const_iterator cbegin() const {

			return elements.cbegin();

		}
		const_iterator cend() const {

			return elements.cend();

		}
		reverse_iterator rbegin() {

			return elements.rbegin();

		}
		reverse_iterator rend() {

			return elements.rend();

		}
		const_reverse_iterator rbegin() const {

			return elements.rbegin();

		}
		const_reverse_iterator rend() const {

			return elements.rend();

		}

		T& operator[](index_t index) {

			assert(index >= 0);
			assert(index < Size);

			return elements[index];

		}
		const T& operator[](index_t index) const {

			assert(index >= 0);
			assert(index < Size);

			return elements[index];

		}

		template<typename U>
		VectorBase<T, Size>& operator=(const VectorBase<U, Size>& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] = static_cast<T>(rhs[i]);

			return *this;

		}

		template<typename U>
		VectorBase<T, Size>& operator+=(const VectorBase<U, Size>& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] += static_cast<T>(rhs[i]);

			return *this;

		}
		template<typename U>
		VectorBase<T, Size>& operator-=(const VectorBase<U, Size>& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] -= static_cast<T>(rhs[i]);

			return *this;

		}
		template<typename U>
		VectorBase<T, Size>& operator*=(const VectorBase<U, Size>& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] *= static_cast<T>(rhs[i]);

			return *this;

		}
		template<typename U>
		VectorBase<T, Size>& operator/=(const VectorBase<U, Size>& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] /= static_cast<T>(rhs[i]);

			return *this;

		}

		template<typename U>
		VectorBase<T, Size>& operator*=(const U& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] *= static_cast<T>(rhs);

			return *this;

		}
		template<typename U>
		VectorBase<T, Size>& operator/=(const U& rhs) {

			for (size_t i = 0; i < size(); ++i)
				(*this)[i] /= static_cast<T>(rhs);

			return *this;

		}

	private:
		container_t elements;

	};

	template<typename T, size_t Size>
	bool operator==(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs) {

		for (size_t i = 0; i < lhs.size(); ++i)
			if (lhs[i] != rhs[i])
				return false;

		return true;

	}
	template<typename T, size_t Size>
	bool operator!=(const VectorBase<T, Size>& lhs, const VectorBase<T, Size>& rhs) {

		return !(lhs == rhs);

	}

	template<typename T, size_t Size, typename U>
	VectorBase<T, Size> operator+(const VectorBase<T, Size>& lhs, const VectorBase<U, Size>& rhs) {

		VectorBase<T, Size> result = lhs;

		return result += rhs;

	}
	template<typename T, size_t Size, typename U>
	VectorBase<T, Size> operator-(const VectorBase<T, Size>& lhs, const VectorBase<U, Size>& rhs) {

		VectorBase<T, Size> result = lhs;

		return result -= rhs;

	}
	template<typename T, size_t Size, typename U>
	VectorBase<T, Size> operator*(const VectorBase<T, Size>& lhs, const VectorBase<U, Size>& rhs) {

		VectorBase<T, Size> result = lhs;

		return result *= rhs;

	}
	template<typename T, size_t Size, typename U>
	VectorBase<T, Size> operator/(const VectorBase<T, Size>& lhs, const VectorBase<U, Size>& rhs) {

		VectorBase<T, Size> result = lhs;

		return result *= rhs;

	}
	template<typename T, size_t Size, typename U>
	VectorBase<T, Size> operator*(const VectorBase<T, Size>& lhs, const U& rhs) {

		VectorBase<T, Size> result = lhs;

		return result *= rhs;

	}
	template<typename T, size_t Size, typename U>
	VectorBase<T, Size> operator/(const VectorBase<T, Size>& lhs, const U& rhs) {

		VectorBase<T, Size> result = lhs;

		return result /= rhs;

	}

	template<typename T, size_t Size>
	VectorBase<T, Size> operator-(const VectorBase<T, Size>& operand) {

		VectorBase<T, Size> result = operand;

		return result *= static_cast<T>(-1);

	}

	template<typename T, size_t Size>
	std::ostream& operator<<(std::ostream& stream, const VectorBase<T, Size>& rhs) {

		stream << '<';

		for (auto i = rhs.begin(); i != rhs.end(); ++i) {

			stream << *i;

			if (std::next(i) != rhs.end())
				stream << ", ";

		}

		stream << '>';

		return stream;

	}

	template<typename T, size_t Size>
	class Vector :
		public VectorBase<T, Size> {

	public:
		using VectorBase<T, Size>::VectorBase;

	};

	template<typename T>
	class Vector<T, 2> :
		public VectorBase<T, 2> {

		using base_t = VectorBase<T, 2>;

	public:
		using VectorBase<T, 2>::VectorBase;

		Vector(T x, T y) :
			VectorBase<T, 2>({ x, y }) {
		}

		T X() const {

			return (*this)[0];

		}
		T Y() const {

			return (*this)[1];

		}

		T Direction() const {

			T radians = std::atan2(-Y(), X());
			T degrees = RadiansToDegrees(radians);

			if (degrees < static_cast<T>(0))
				degrees += static_cast<T>(360);

			return std::abs(degrees);

		}
		T Angle() const {

			T radians = std::atan(-Y() / X());
			T degrees = RadiansToDegrees(radians);

			if (degrees < static_cast<T>(0))
				degrees += static_cast<T>(90);

			return std::abs(degrees);

		}
		Quadrant Quadrant() const {

			T zero = static_cast<T>(0);

			if (X() > zero && -Y() > zero)
				return Quadrant::First;
			else if (X() < zero && -Y() > zero)
				return Quadrant::Second;
			else if (X() < zero && -Y() < zero)
				return Quadrant::Third;
			else if (X() > zero && -Y() < zero)
				return Quadrant::Fourth;
			else
				return Quadrant::None;

		}
		T Determinant(const base_t& other) const {

			return (X() * other[1]) - (Y() * other[0]);

		}

		template<typename U>
		T CrossProduct(const VectorBase<U, 2>& other) const {

			Vector<T, 3> lhs{ X(), Y(), static_cast<T>(0) };
			Vector<T, 3> rhs{ static_cast<T>(other[0]), static_cast<T>(other[1]), static_cast<T>(0) };

			return lhs.CrossProduct(rhs).Z();

		}

	};

	template<typename T>
	class Vector<T, 3> :
		public VectorBase<T, 3> {

		using base_t = VectorBase<T, 3>;

	public:
		using VectorBase<T, 3>::VectorBase;

		Vector(T x, T y, T z) :
			VectorBase<T, 3>({ x, y, z }) {
		}

		T X() const {

			return (*this)[0];

		}
		T Y() const {

			return (*this)[1];

		}
		T Z() const {

			return (*this)[2];

		}

		template<typename U>
		Vector CrossProduct(const VectorBase<U, 3>& other) const {

			T x1 = (*this)[0];
			T y1 = (*this)[1];
			T z1 = (*this)[2];

			T x2 = static_cast<T>(other[0]);
			T y2 = static_cast<T>(other[1]);
			T z2 = static_cast<T>(other[2]);

			return Vector(
				(y1 * z2) - (z1 * y2),
				(z1 * x2) - (x1 * z2),
				(x1 * y2) - (y1 * x2)
			);

		}

	};

	template<typename T>
	using Vector2 = Vector<T, 2>;
	template<typename T>
	using Vector3 = Vector<T, 3>;

	using Vector2f = Vector2<float>;
	using Vector3f = Vector3<float>;
	using Vector2i = Vector2<int>;
	using Vector3i = Vector3<int>;

}