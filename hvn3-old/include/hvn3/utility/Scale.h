#pragma once

#include "hvn3/math/Point2d.h"
#include "hvn3/utility/Size.h"

namespace hvn3 {

	class Scale {

	public:
		typedef float value_type;

		Scale(value_type scale);
		Scale(value_type x_scale, value_type y_scale);

		template <typename T>
		Scale(const Size<T>& a, const Size<T>& b) :
			Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
		}

		value_type XScale() const;
		value_type YScale() const;
		value_type Factor() const;

		template <typename T>
		Point2d<T> ScalePoint(const Point2d<T>& point) const {

			return Point2d<T>(
				point.X() * _scale_factor_x,
				point.Y() * _scale_factor_y
				);

			return point;

		}
		void ScalePoint(value_type& x, value_type& y) const;

		Scale Inverse() const;

		friend Scale operator*(const Scale& a, const Scale& b);
		Scale& operator*=(const Scale& other);

	private:
		value_type _scale_factor_x;
		value_type _scale_factor_y;

	};

	template <typename T>
	Size<T> operator*(const Size<T>& lhs, const Scale& rhs) {

		Size<T> out = lhs;

		out.width = static_cast<typename Size<T>::value_type>(static_cast<Scale::value_type>(out.width) * rhs.XScale());
		out.height = static_cast<typename Size<T>::value_type>(static_cast<Scale::value_type>(out.height) * rhs.YScale());

		return out;

	}
	template <typename T>
	Size<T>& operator*=(Size<T>& lhs, const Scale& rhs) {

		lhs = lhs * rhs;

		return lhs;

	}
	template <typename T>
	Point2d<T> operator*(const Point2d<T>& lhs, const Scale& rhs) {

		Point2d<T> out = lhs;

		out.x = static_cast<typename Point2d<T>::value_type>(static_cast<Scale::value_type>(out.x) * rhs.XScale());
		out.y = static_cast<typename Point2d<T>::value_type>(static_cast<Scale::value_type>(out.y) * rhs.YScale());

		return out;

	}
	template <typename T>
	Point2d<T>& operator*=(Point2d<T>& lhs, const Scale& rhs) {

		lhs = lhs * rhs;

		return lhs;

	}

}