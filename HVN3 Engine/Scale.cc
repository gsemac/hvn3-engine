#include "Scale.h"

namespace hvn3 {

	Scale::Scale(float scale) :
		Scale(scale, scale) {
	}
	Scale::Scale(float x_scale, float y_scale) {

		__scale_factor_x = x_scale;
		__scale_factor_y = y_scale;

	}
	Scale::Scale(const Rectangle& a, const Rectangle& b) :
		Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
	}
	Scale::Scale(const Size& a, const Size& b) :
		Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
	}
	Scale::Scale(const ISizeable& a, const ISizeable& b) :
		Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
	}

	float Scale::XScale() const {

		return __scale_factor_x;

	}
	float Scale::YScale() const {

		return __scale_factor_y;

	}
	Point& Scale::ScalePoint(Point& point) const {

		point.SetX(point.X() * __scale_factor_x);
		point.SetY(point.Y() * __scale_factor_y);

		return point;

	}
	void Scale::ScalePoint(float& x, float& y) const {

		x *= __scale_factor_x;
		y *= __scale_factor_y;

	}
	Scale Scale::Inverse() const {

		return Scale(1.0f / XScale(), 1.0f / YScale());

	}

	Scale operator*(const Scale& a, const Scale& b) {

		return Scale(a.XScale() * b.XScale(), a.YScale() * b.YScale());

	}
	Scale& Scale::operator*=(const Scale& other) {

		__scale_factor_x *= other.__scale_factor_x;
		__scale_factor_y *= other.__scale_factor_y;

		return *this;

	}

}