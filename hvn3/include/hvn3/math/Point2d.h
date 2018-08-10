#pragma once
#include "hvn3/utility/Hash.h"
#include <iostream>

namespace hvn3 {

	template<typename T>
	class Rectangle;

	template <typename T>
	class Point2d {

	public:
		Point2d() :
			Point2d(0, 0) {
		}
		Point2d(T x, T y) :
			x(x),
			y(y) {
		}

		T X() const {
			return x;
		}
		T Y() const {
			return y;
		}

		void SetX(T x) {
			this->x = x;
		}
		void SetY(T y) {
			this->y = y;
		}

		void Offset(T x, T y) {
			this->x += x;
			this->y += y;
		}
		void Offset(const Point2d<T>& offset) {
			this->x += offset.x;
			this->y += offset.y;
		}

		bool IsEmpty() const {

			return (x == 0 && y == 0);

		}
		bool In(const Rectangle<T>& rectangle) const {
			return (x >= rectangle.X() && x < rectangle.X2() && y >= rectangle.Y() && y < rectangle.Y2());
		}

		static Point2d<T> Empty() {

			return Point2d<T>(0, 0);

		}
		static Point2d<T> Round(const Point2d<T>& point) {

			return Point2d<T>(std::round(point.X()), std::round(point.Y()));

		}
		static Point2d<T> Ceiling(const Point2d<T>& point) {

			return Point2d<T>(std::ceil(point.X()), std::ceil(point.Y()));

		}
		static Point2d<T> Truncate(const Point2d<T>& point) {

			return Point2d<T>(std::trunc(point.X()), std::trunc(point.Y()));

		}

		Point2d<T>& operator+=(const Point2d<T>& other) {

			Offset(other);

			return *this;

		}
		Point2d<T>& operator-=(const Point2d<T>& other) {

			Offset(-other);

			return *this;

		}
		Point2d<T>& operator*=(const T other) {

			x *= other;
			y *= other;

			return *this;

		}
		Point2d<T>& operator/=(const T other) {

			x /= other;
			y /= other;

			return *this;

		}
		Point2d<T> operator*(T other) const {
			return Point2d<T>(x * other, y * other);
		}
		Point2d<T> operator/(T other) const {
			return Point2d<T>(x / other, y / other);
		}

		friend bool operator==(const Point2d<T>& a, const Point2d<T>& b) {

			return (a.X() == b.X()) && (a.Y() == b.Y());

		}
		friend bool operator!=(const Point2d<T>& a, const Point2d<T>& b) {

			return !(a == b);

		}
		friend Point2d<T> operator+(const Point2d<T>& a, const Point2d<T>& b) {

			return Point2d<T>(a.X() + b.X(), a.Y() + b.Y());

		}
		friend Point2d<T> operator-(const Point2d<T>& a, const Point2d<T>& b) {

			return Point2d<T>(a.X() - b.X(), a.Y() - b.Y());

		}
		friend Point2d<T> operator-(const Point2d<T>& a) {

			return Point2d<T>(-a.X(), -a.Y());

		}
		friend Point2d<T> operator*(const T other, const Point2d<T>& rhs) {

			return Point2d<T>(rhs.x * other, rhs.y * other);

		}
		friend Point2d<T> operator/(const T other, const Point2d<T>& rhs) {

			return Point2d<T>(rhs.x / other, rhs.y / other);

		}

		template<typename U>
		operator Point2d<U>() {

			return Point2d<U>(static_cast<U>(x), static_cast<U>(y));

		}

		T x;
		T y;

	};

	typedef Point2d<float> PointF;
	typedef Point2d<int> PointI;

	template <typename T>
	std::ostream& operator<< (std::ostream& stream, const hvn3::Point2d<T>& point) {

		stream << "(" << point.X() << ", " << point.Y() << ")";

		return stream;

	}

}

namespace std {

	template <typename T>
	struct hash<hvn3::Point2d<T>> {

		std::size_t operator()(const hvn3::Point2d<T>& point) const {

			size_t seed = 0;
			hvn3::hash_combine(seed, point.X());
			hvn3::hash_combine(seed, point.Y());
			return seed;

		}

	};

}