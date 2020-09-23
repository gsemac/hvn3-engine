#include "hvn3/utility/Scale.h"

namespace hvn3 {

	Scale::Scale(value_type scale) :
		Scale(scale, scale) {
	}
	Scale::Scale(value_type x_scale, value_type y_scale) {

		_scale_factor_x = x_scale;
		_scale_factor_y = y_scale;

	}

	Scale::value_type Scale::XScale() const {

		return _scale_factor_x;

	}
	Scale::value_type Scale::YScale() const {

		return _scale_factor_y;

	}
	Scale::value_type Scale::Factor() const {

		return (_scale_factor_x + _scale_factor_y) / 2.0f;

	}

	void Scale::ScalePoint(value_type& x, value_type& y) const {

		x *= _scale_factor_x;
		y *= _scale_factor_y;

	}
	Scale Scale::Inverse() const {

		return Scale(1.0f / XScale(), 1.0f / YScale());

	}

	Scale operator*(const Scale& a, const Scale& b) {

		return Scale(a.XScale() * b.XScale(), a.YScale() * b.YScale());

	}
	Scale& Scale::operator*=(const Scale& other) {

		_scale_factor_x *= other._scale_factor_x;
		_scale_factor_y *= other._scale_factor_y;

		return *this;

	}

}