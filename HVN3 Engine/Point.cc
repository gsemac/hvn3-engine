#include "Point.h"

namespace hvn3 {

	Point::Point(float x, float y) : IPositionable(x, y) {}
	bool Point::IsEmpty() const {

		return (X() == 0.0f && Y() == 0.0f);

	}
	void Point::Round() {

		SetXY(std::roundf(X()), std::roundf(Y()));

	}
	void Point::Ceiling() {

		SetXY(std::ceilf(X()), std::ceilf(Y()));

	}
	void Point::Truncate() {

		SetXY(std::truncf(X()), std::truncf(Y()));

	}

	bool operator==(const Point& a, const Point& b) {

		return (a.X() == b.X()) && (a.Y() == b.Y());

	}
	bool operator!=(const Point& a, const Point& b) {

		return !(a == b);

	}
	Point operator+(const Point& a, const Point& b) {

		return Point(a.X() + b.X(), a.Y() + b.Y());

	}
	Point operator-(const Point& a, const Point& b) {

		return Point(a.X() - b.X(), a.Y() - b.Y());

	}
	Point operator-(const Point& a) {

		return Point(-a.X(), -a.Y());

	}
	Point& Point::operator+=(const Point& other) {

		Translate(other.X(), other.Y());
		return *this;

	}
	Point& Point::operator-=(const Point& other) {

		Translate(-other.X(), -other.Y());
		return *this;

	}

	std::ostream& operator<< (std::ostream& stream, const Point& point) {

		stream << "(" << point.X() << ", " << point.Y() << ")";
		return stream;

	}

}