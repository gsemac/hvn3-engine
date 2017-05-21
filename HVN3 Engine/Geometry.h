#pragma once
#include "Rectangle.h"

struct Circle : public IPositionable {

private:
	float __radius;

public:
	Circle(float radius);
	Circle(float x, float y, float radius);
	Circle(Point pos, float radius);
	float Radius() const;

};

struct Line {

private:
	Point __a, __b;

public:
	Line(float x1, float y1, float x2, float y2);
	Line(const Point& a, const Point& b);
	const Point& First() const;
	const Point& Second() const;
	float Length() const;
	
};

bool PointIn(const Point& point, const Rectangle& rect);
float PointDistance(const Point& a, const Point& b);
float PointDistance(const Point& point, const Line& line);
float PointDistance(const Point& point, const Rectangle& rect);
float PointDistanceSquared(const Point& a, const Point& b);
float PointDistanceSquared(const Point& point, const Rectangle& rect);
Point PointInDirection(const Point& point, float degrees, float distance);
float PointDirection(const Point& a, const Point& b);
float PointDirection(float ax, float ay, float bx, float by);
void PointRotate(Point& point, const Point& origin, float angle);
IPositionable& Translate(IPositionable& obj, float x_offset, float y_offset);
float Distance(const Rectangle& a, const Rectangle& b);

bool Intersects(const Rectangle& a, const Rectangle& b);
bool Intersects(const Circle& a, const Line& b);
bool Intersects(const Rectangle& a, const Circle& b);
bool Intersects(const Rectangle& a, const Line& b);
bool Intersects(const Circle& a, const Circle& b);
bool Intersects(const Line& a, const Line& b);