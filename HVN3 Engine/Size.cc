#include "Size.h"
#include "Point.h"

Size::Size(float width, float height) : __width(width), __height(height) {}

float Size::Width() const {
	return __width;
}
float Size::Height() const {
	return __height;
}
void Size::Resize(float width, float height) {
	__width = width;
	__height = height;
}

bool operator<(const Size& a, const Size& b) {

	return (a.Width() * b.Height() < b.Width() * b.Height());

}
bool operator>(const Size& a, const Size& b) {

	return (a.Width() * b.Height() > b.Width() * b.Height());

}
bool operator<=(const Size& a, const Size& b) {

	return (a < b || a == b);

}
bool operator>=(const Size& a, const Size& b) {

	return (a > b || a == b);

}
bool operator==(const Size& a, const Size& b) {

	return (a.Width() == b.Width() && a.Height() == b.Height());

}