#include "Size.h"
#include "Point.h"

namespace hvn3 {

	Size::Size(float width, float height) : __width(width), __height(height) {}

	float Size::Width() const {
		return __width;
	}
	float Size::Height() const {
		return __height;
	}
	void Size::SetWidth(float value) {

		Resize(value, __height);

	}
	void Size::SetHeight(float value) {

		Resize(__width, value);

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

	std::ostream& operator<< (std::ostream& stream, const Size& size) {

		stream << size.Width() << 'x' << size.Height();
		return stream;

	}

}