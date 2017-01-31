#pragma once
#include "Point.h"
#include "ISizeable.h"

struct Rectangle : public IPositionable, public ISizeable {

public:
	Rectangle(float width, float height);
	Rectangle(float x, float y, float width, float height);
	Rectangle(Point top_left, Point bottom_right);
	float X2() const;
	float Y2() const;

	float Bottom() const;
	float Top() const;
	float Left() const;
	float Right() const;
	Point TopLeft() const;
	Point TopRight() const;
	Point BottomLeft() const;
	Point BottomRight() const;
	Point Midpoint() const;
	Size Size() const;

	void Translate(float x_offset, float y_offset);
	void Scale(float x_scale, float y_scale);
	void Move(float x, float y);

	static Rectangle Intersect(const Rectangle& a, const Rectangle& b);

	float Area() const;

};

bool operator>(const Rectangle &a, const Rectangle &b);
bool operator<(const Rectangle &a, const Rectangle &b);