#pragma once
#include "Positionable2dBase.h"
#include "ISizeable.h"

namespace hvn3 {

	template <typename T>
	struct Rectangle : public Positionable2dBase<T>, public ISizeable<T> {

	public:
		Rectangle(T width, T height) :
			Rectangle(0, 0, width, height) {}
		Rectangle(T x, T y, T width, T height) :
			Positionable2dBase<T>(x, y),
			ISizeable<T>(width, height) {}
		Rectangle(const Point2d<T>& top_left, const Point2d<T>& bottom_right) :
			Rectangle(top_left.X(), top_left.Y(), bottom_right.X() - top_left.X(), bottom_right.Y() - top_left.Y()) {}

		T X2() const {

			return X() + Width();

		}
		T Y2() const {

			return Y() + Height();

		}

		T Bottom() const {

			return Y2();

		}
		T Top() const {

			return Y();

		}
		T Left() const {

			return X();

		}
		T Right() const {

			return X2();

		}
		Point2d<T> TopLeft() const {

			return Point2d<T>(X(), Y());

		}
		Point2d<T> TopRight() const {

			return Point2d<T>(X() + Width(), Y());

		}
		Point2d<T> BottomLeft() const {

			return Point2d<T>(X(), Y() + Height());

		}
		Point2d<T> BottomRight() const {

			return Point2d<T>(X() + Width(), Y() + Height());

		}
		Point2d<T> Midpoint() const {

			return Point2d<T>(X() + Width() / 2.0f, Y() + Height() / 2.0f);

		}

		void Translate(T x_offset, T y_offset) {

			SetPosition(X() + x_offset, Y() + y_offset);

		}
		void Scale(T x_scale, T y_scale) {

			throw NotImplementedException();

		}

		static Rectangle Intersection(const Rectangle<T>& a, const Rectangle<T>& b) {

			Point2d<T> tl((std::max)(a.Left(), b.Left()), (std::max)(a.Top(), b.Top()));
			Point2d<T> br((std::min)(a.Right(), b.Right()), (std::min)(a.Bottom(), b.Bottom()));

			return Rectangle<T>(tl, br);

		}

		T Area() const {

			return Width() * Height();

		}

	};
	
	template <typename T>
	bool operator>(const Rectangle<T>& a, const Rectangle<T>& b) {

		return a.Area() > b.Area();

	}
	template <typename T>
	bool operator<(const Rectangle<T>& a, const Rectangle<T>& b) {

		return a.Area() < b.Area();

	}
	
	typedef Rectangle<float> RectangleF;
	typedef Rectangle<int> RectangleI;

}