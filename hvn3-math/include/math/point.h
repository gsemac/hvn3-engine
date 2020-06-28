#pragma once

#include "math/vector.h"
#include "utilities/hash_builder.h"

#include <cstddef>
#include <functional>
#include <iostream>

namespace hvn3::math {

	template<typename T>
	struct Point2 {

		T x;
		T y;

		Point2() :
			Point2(static_cast<T>(0), static_cast<T>(0)) {
		}
		Point2(T x, T y) {

			this->x = x;
			this->y = y;

		}
		Point2(const Point2<T>& other) :
			Point2(other.X(), other.Y()) {
		}

		T X() const {

			return x;

		}
		T Y() const {

			return y;

		}

		void SetX(T value) {

			x = value;

		}
		void SetY(T value) {

			y = value;

		}
		void SetXY(T x, T y) {

			SetX(x);
			SetY(y);

		}

		Point2<T> Translate(T xOffset, T yOffset) const {

			return Point2<T>(X() + xOffset, Y() + yOffset);

		}
		Point2<T> Translate(const Point2<T>& offset) const {

			return Translate(offset.X(), offset.Y());

		}
		Point2<T> Translate(const VectorBase<T, 2>& vector) const {

			return Translate(vector[0], vector[1]);

		}

		Vector2<T> To(const Point2<T>& other) const {

			T x = other.X() - X();
			T y = other.Y() - Y();

			return Vector2<T>(x, y);

		}

		bool IsEmpty() const {

			return X() == static_cast<T>(0) &&
				Y() == static_cast<T>(0);

		}

		static Point2<T> Empty() {

			return Point2<T>();

		}

		template<typename U>
		Point2<T>& operator=(const Point2<U>& other) {

			SetXY(static_cast<T>(other.X()), static_cast<T>(other.Y()));

			return *this;

		}

		template<typename U>
		Point2<T>& operator+=(const Point2<U>& other) {

			SetXY(X() + static_cast<T>(other.X()), Y() + static_cast<T>(other.Y()));

			return *this;

		}
		template<typename U>
		Point2<T>& operator-=(const Point2<U>& other) {

			SetXY(X() - static_cast<T>(other.X()), Y() - static_cast<T>(other.Y()));

			return *this;

		}
		template<typename U>
		Point2<T>& operator*=(const Point2<U>& other) {

			SetXY(X() * static_cast<T>(other.X()), Y() * static_cast<T>(other.Y()));

			return *this;

		}
		template<typename U>
		Point2<T>& operator/=(const Point2<U>& other) {

			SetXY(X() / static_cast<T>(other.X()), Y() / static_cast<T>(other.Y()));

			return *this;

		}

		template<typename U>
		Point2<T>& operator*=(const U& other) {

			SetXY(X() * static_cast<T>(other), Y() * static_cast<T>(other));

			return *this;

		}
		template<typename U>
		Point2<T>& operator/=(const U& other) {

			SetXY(X() / static_cast<T>(other), Y() / static_cast<T>(other));

			return *this;

		}

	};

	template<typename T>
	bool operator==(const Point2<T>& lhs, const Point2<T>& rhs) {

		return lhs.X() == rhs.X() &&
			lhs.Y() == rhs.Y();

	}
	template<typename T>
	bool operator!=(const Point2<T>& lhs, const Point2<T>& rhs) {

		return !(lhs == rhs);

	}

	template<typename T, typename U>
	Point2<T> operator+(const Point2<T>& lhs, const Point2<U>& rhs) {

		Point2<T> result(lhs);

		return lhs += rhs;

	}
	template<typename T, typename U>
	Point2<T> operator-(const Point2<T>& lhs, const Point2<U>& rhs) {

		Point2<T> result(lhs);

		return lhs -= rhs;

	}
	template<typename T, typename U>
	Point2<T> operator*(const Point2<T>& lhs, const Point2<U>& rhs) {

		Point2<T> result(lhs);

		return lhs *= rhs;

	}
	template<typename T, typename U>
	Point2<T> operator/(const Point2<T>& lhs, const Point2<U>& rhs) {

		Point2<T> result(lhs);

		return lhs /= rhs;

	}

	template <typename T>
	std::ostream& operator<<(std::ostream& stream, const Point2<T>& rhs) {

		stream << "(" << point.X() << ", " << point.Y() << ")";

		return stream;

	}

	using Point2f = Point2<float>;
	using Point2i = Point2<int>;

}

namespace std {

	template <typename T>
	struct hash<hvn3::math::Point2<T>> {

		size_t operator()(const hvn3::math::Point2<T>& point) const {

			hvn3::utilities::HashBuilder hashBuilder;

			hashBuilder.Add(point.X());
			hashBuilder.Add(point.Y());

			return hashBuilder.Build();

		}

	};

}