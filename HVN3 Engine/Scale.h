#pragma once
#include "Rectangle.h"

namespace hvn3 {

	class Scale {

	public:
		Scale(float scale);
		Scale(float x_scale, float y_scale);

		template <typename T>
		Scale(const Rectangle<T>& a, const Rectangle<T>& b) :
			Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
		}
		template <typename T>
		Scale(const Size<T>& a, const Size<T>& b) :
			Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
		}
		template <typename T>
		Scale(const ISizeable<T>& a, const ISizeable<T>& b) :
			Scale(a.Width() / b.Width(), a.Height() / b.Height()) {
		}

		float XScale() const;
		float YScale() const;

		template <typename T>
		Point2d<T>& ScalePoint(Point2d<T>& point) const {

			point.SetX(point.X() * _scale_factor_x);
			point.SetY(point.Y() * _scale_factor_y);

			return point;

		}
		void ScalePoint(float& x, float& y) const;

		Scale Inverse() const;

		friend Scale operator*(const Scale& a, const Scale& b);
		Scale& operator*=(const Scale& other);

	private:
		float _scale_factor_x;
		float _scale_factor_y;

	};

}