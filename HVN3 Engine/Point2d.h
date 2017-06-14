#pragma once
#include <iostream>
#include "Hash.h"

namespace hvn3 {

	template <typename T>
	class Point2d {

	public:
		Point2d() :
			Point2d(0, 0) {}
		Point2d(T x, T y) {

			_x = x;
			_y = y;

		}

		T X() const {

			return _x;

		}
		T Y() const {

			return _y;

		}

		void SetX(T x) {

			_x = x;

		}
		void SetY(T y) {

			_y = y;

		}

		void Offset(T x, T y) {

			_x += x;
			_y += y;

		}
		void Offset(const Point2d<T>& offset) {

			_x += offset.X();
			_y += offset.Y();

		}

		bool IsEmpty() const {

			return (!_x && !_y);

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
		
		friend bool operator==(const Point2d<T>& a, const Point2d<T>& b) {

			return (a.X() == b.X()) && (a.Y() == b.Y());

		}
		friend bool operator!=(const Point2d<T>& a, const Point2d<T>& b) {

			return !(a == b);

		}
		friend Point2d<T> operator+(const Point2d<T>& a, const Point2d<T>& b) {

			return Point(a.X() + b.X(), a.Y() + b.Y());

		}
		friend Point2d<T> operator-(const Point2d<T>& a, const Point2d<T>& b) {

			return Point(a.X() - b.X(), a.Y() - b.Y());

		}
		friend Point2d<T> operator-(const Point2d<T>& a) {

			return Point2d<T>(-a.X(), -a.Y());

		}
		Point2d<T>& operator+=(const Point2d<T>& other) {

			Offset(other);

			return *this;

		}
		Point2d<T>& operator-=(const Point2d<T>& other) {

			Offset(-other);

			return *this;

		}

	private:
		T _x;
		T _y;

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
			hash_combine(seed, point.X());
			hash_combine(seed, point.Y());
			return seed;

		}

	};

}