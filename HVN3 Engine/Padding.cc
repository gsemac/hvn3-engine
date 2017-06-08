#include "Padding.h"

namespace hvn3 {

	// Public methods

	Padding::Padding() :
		Padding(0.0f) {}
	Padding::Padding(float all) :
		Padding(all, all, all, all) {}
	Padding::Padding(float left, float right, float top, float bottom) {

		_left = left;
		_right = right;
		_top = top;
		_bottom = bottom;

	}

	// Private methods

	float Padding::Left() const {

		return _left;

	}
	float Padding::Right() const {

		return _right;

	}
	float Padding::Top() const {

		return _top;

	}
	float Padding::Bottom() const {

		return _bottom;

	}

}