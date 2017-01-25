#pragma once
#include "Rectangle.h"

Rectangle::Rectangle(float X, float Y, float width, float height) : IPositionable(X, Y), ISizeable(width, height) {}
Rectangle::Rectangle(float width, float height) : Rectangle(0, 0, width, height) {}
Rectangle::Rectangle(Point top_left, Point bottom_right) : Rectangle(top_left.X(), top_left.Y(), bottom_right.X() - top_left.X(), bottom_right.Y() - top_left.Y()) {}
float Rectangle::X2() const {

	return X() + Width();

}
float Rectangle::Y2() const {

	return Y() + Height();

}
float Rectangle::Bottom() const {
	return Y2();
}
float Rectangle::Top() const {
	return Y();
}
float Rectangle::Left() const {
	return X();
}
float Rectangle::Right() const {
	return X2();
}
Point Rectangle::TopLeft() const {

	return Point(X(), Y());

}
Point Rectangle::TopRight() const {

	return Point(X() + Width(), Y());

}
Point Rectangle::BottomLeft() const {

	return Point(X(), Y() + Height());

}
Point Rectangle::BottomRight() const {

	return Point(X() + Width(), Y() + Height());

}
Point Rectangle::Midpoint() const {

	return Point(X() + Width() / 2.0f, Y() + Height() / 2.0f);

}
Size Rectangle::Size() const {

	return ::Size(Width(), Height());

}
void Rectangle::Translate(float x_offset, float y_offset) {

	IPositionable::Translate(x_offset, y_offset);

}
void Rectangle::Scale(float x_scale, float y_scale) {}
void Rectangle::Move(float x, float y) {

	SetXY(x, y);

}
float Rectangle::Area() const {

	return Width() * Height();

}

bool operator>(const Rectangle &a, const Rectangle &b) {

	return a.Area() > b.Area();

}
bool operator<(const Rectangle &a, const Rectangle &b) {

	return a.Area() < b.Area();

}