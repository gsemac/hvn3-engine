#include "Point.h"

Point::Point(float x, float y) : IPositionable(x, y) {}
bool Point::IsEmpty() const  {

	return (X == 0.0f && Y == 0.0f);

}
void Point::Round() {

	X = std::roundf(X);
	Y = std::roundf(Y);

}
void Point::Ceiling() {

	X = std::ceilf(X);
	Y = std::ceilf(Y);

}
void Point::Truncate() {

	X = std::truncf(X);
	Y = std::truncf(Y);

}

bool operator==(const Point& a, const Point& b) {

	return (a.X == b.X) && (a.Y == b.Y);

}
bool operator!=(const Point& a, const Point& b) {

	return !(a == b);

}
Point operator+(const Point& a, const Point& b) {

	return Point(a.X + b.X, a.Y + b.Y);

}
Point operator-(const Point& a, const Point& b) {

	return Point(a.X - b.X, a.Y - b.Y);

}
Point& Point::operator+=(const Point& other) {

	X += other.X;
	Y += other.Y;
	return *this;

}
Point& Point::operator-=(const Point& other) {

	X -= other.X;
	Y -= other.Y;
	return *this;

}

std::ostream& operator<< (std::ostream& stream, const Point& point) {

	stream << "(" << point.X << ", " << point.Y << ")";
	return stream;

}