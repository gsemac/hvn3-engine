#include <cmath>
#include <algorithm>
#include "Geometry.h"
#include "Sprite.h"
#include "Utility.h"

// Class Functions

Circle::Circle(float radius) : Circle(0.0f, 0.0f, radius) {}
Circle::Circle(float x, float y, float radius) : IPositionable(x, y) {

	__radius = radius;

}
Circle::Circle(Point pos, float radius) : Circle(pos.X(), pos.Y(), radius) {}
float Circle::Radius() const {

	return __radius;

}

Line::Line(float x1, float y1, float x2, float y2) : __a(x1, y1), __b(x2, y2) {}
Line::Line(const Point& a, const Point& b) : __a(a), __b(b) {}
const Point& Line::First() const {
	return __a;
}
const Point& Line::Second() const {
	return __b;
}
float Line::Length() const {

	return PointDistance(__a, __b);

}

// Non-Class Functions

bool PointIn(const Point& point, const Rectangle& rect) {

	return (point.X() >= rect.X() && point.X() < rect.X2() && point.Y() >= rect.Y() && point.Y() < rect.Y2());

}
float PointDistance(const Point& a, const Point& b) {

	return std::sqrt(PointDistanceSquared(a, b));

}
float PointDistance(const Point& point, const Line& line) {

	float a = point.X() - line.First().X();
	float b = point.Y() - line.First().Y();
	float c = line.Second().X() - line.First().X();
	float d = line.Second().Y() - line.First().Y();

	float dot = a * c + b * d;
	float len_sq = c * c + d * d;
	float param = -1.0f;
	if (len_sq != 0)
		param = dot / len_sq;

	float xx, yy;

	if (param < 0) {
		xx = line.First().X();
		yy = line.First().Y();
	}
	else if (param > 1) {
		xx = line.Second().X();
		yy = line.Second().Y();
	}
	else {
		xx = line.First().X() + param * c;
		yy = line.First().Y() + param * d;
	}

	float dx = point.X() - xx;
	float dy = point.Y() - yy;

	return std::sqrt(dx * dx + dy * dy);


}
float PointDistance(const Point& point, const Rectangle& rect) {

	return (std::sqrtf)(PointDistanceSquared(point, rect));

}
float PointDistanceSquared(const Point& a, const Point& b) {

	float dx = b.X() - a.X();
	float dy = b.Y() - a.Y();
	return dx * dx + dy * dy;

}
float PointDistanceSquared(const Point& point, const Rectangle& rect) {

	int dx = (std::max)((std::abs)(point.X() - rect.X() + rect.Width() / 2.0f) - rect.Width() / 2.0f, 0.0f);
	int dy = (std::max)((std::abs)(point.Y() - rect.Y() + rect.Height() / 2.0f) - rect.Height() / 2.0f, 0.0f);

	return dx * dx + dy * dy;

}
Point PointInDirection(const Point& point, float degrees, float distance) {

	float rad = DegreesToRadians(degrees);
	return Point(point.X() + std::cos(rad) * distance, point.Y() - std::sin(rad) * distance);

}
float PointDirection(const Point& a, const Point& b) {

	return PointDirection(a.X(), a.Y(), b.X(), b.Y());
	
}
float PointDirection(float ax, float ay, float bx, float by) {

	float angle = RadiansToDegrees((std::atan2)(ay - by, bx - ax));
	if (angle < 0.0f) angle += 360.0f;

	return angle;

}
void PointRotate(Point& point, const Point& origin, float angle) {

	float rad = DegreesToRadians(angle);
	float s = std::sin(rad);
	float c = std::cos(rad);

	// Translate the Point to the origin.
	point -= origin;

	// Rotate the Point.
	float xnew = point.X() * c - point.Y() * s;
	float ynew = point.X() * s + point.Y() * c;

	// Translate the Point back.
	point.SetXY(xnew + origin.X(), ynew + origin.Y());

}
IPositionable& Translate(IPositionable& obj, float x_offset, float y_offset) {

	obj.SetX(obj.X() + x_offset);
	obj.SetY(obj.Y() + y_offset);

	return obj;

}
float Distance(const Rectangle& a, const Rectangle& b) {

	// If the two rectangles intersect, the distance between them is zero.
	if (Intersects(a, b)) return 0.0f;

	// Otherwise, the distance is the distance to the closest corner of the other rectangle.
	bool left = b.Right() < a.Left(); // b is on the left of a
	bool right = b.Left() > a.Right(); // b is on the right of a
	bool bottom = b.Top() > a.Bottom(); // b is below a
	bool top = b.Bottom() < a.Top(); // b is above a

	if (top && left)
		return PointDistance(b.BottomRight(), a.TopLeft());
	else if (left && bottom)
		return PointDistance(b.TopRight(), a.BottomLeft());
	else if (bottom && right)
		return PointDistance(b.TopLeft(), a.BottomRight());
	else if (right && top)
		return PointDistance(b.BottomLeft(), a.TopRight());
	else if (left)
		return a.Left() - b.Right();
	else if (right)
		return b.Left() - a.Right();
	else if (bottom)
		return b.Top() - a.Bottom();
	else
		return a.Top() - b.Bottom();

}

// Helper Functions

bool Intersects(const Rectangle& a, const Rectangle& b) {
	
	return (a.X() < b.X2() && a.X2() > b.X() &&	a.Y() < b.Y2() && a.Y2() > b.Y());

}
bool Intersects(const Circle& a, const Line& b) {

	// Note: This procedure uses the same logic as PointDistance, but avoids using the costly sqrt function.

	float _a = a.X() - b.First().X();
	float _b = a.Y() - b.First().Y();
	float _c = b.Second().X() - b.First().X();
	float _d = b.Second().Y() - b.First().Y();

	float dot = _a * _c + _b * _d;
	float len_sq = _c * _c + _d * _d;
	float param = -1.0f;
	if (len_sq != 0)
		param = dot / len_sq;

	float xx, yy;

	if (param < 0) {
		xx = b.First().X();
		yy = b.First().Y();
	}
	else if (param > 1) {
		xx = b.Second().X();
		yy = b.Second().Y();
	}
	else {
		xx = b.First().X() + param * _c;
		yy = b.First().Y() + param * _d;
	}

	float dx = a.X() - xx;
	float dy = a.Y() - yy;

	return (dx * dx + dy * dy) < (std::pow)(a.Radius(), 2.0f);

	// return PointDistance(Point(a.X(), a.Y), b) <= a.Radius();

}
bool Intersects(const Rectangle& a, const Circle& b) {

	return PointIn(Point(b.X(), b.Y()), a) ||
		Intersects(b, Line(a.X(), a.Y(), a.X2(), a.Y())) || // top
		Intersects(b, Line(a.X(), a.Y2(), a.X2(), a.Y2())) || // bottom 
		Intersects(b, Line(a.X(), a.Y(), a.X(), a.Y2())) || // left
		Intersects(b, Line(a.X2(), a.Y(), a.X2(), a.Y2())); // right

}
bool Intersects(const Rectangle& a, const Line& b) {
	return false;
}
bool Intersects(const Circle& a, const Circle& b) {

	return PointDistance(Point(a.X(), a.Y()), Point(b.X(), b.Y())) < (a.Radius() + b.Radius());

}
bool Intersects(const Line& a, const Line& b) {

	return false;

}