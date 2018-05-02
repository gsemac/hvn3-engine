#pragma once
#include "hvn3/core/Positionable2dBase.h"
#include "hvn3/core/SizeableBase.h"

namespace hvn3 {

	enum class CropSide {
		Top,
		Left,
		Right,
		Bottom
	};

	template <typename T>
	struct Rectangle : public Positionable2dBase<T>, public SizeableBase<T> {

	public:
		Rectangle(T width, T height) :
			Rectangle<T>(0, 0, width, height) {
		}
		Rectangle(hvn3::Size<T> size) :
			Rectangle(size.width, size.height) {
		}
		Rectangle(T x, T y, T width, T height) :
			Positionable2dBase<T>(x, y),
			SizeableBase<T>(width, height) {}
		Rectangle(const Point2d<T>& top_left, const Point2d<T>& bottom_right) :
			Rectangle<T>(top_left.X(), top_left.Y(), bottom_right.X() - top_left.X(), bottom_right.Y() - top_left.Y()) {
		}
		Rectangle(const Point2d<T>& position, const hvn3::Size<T>& size) :
			Rectangle(position.X(), position.Y(), size.Width(), size.Height()) {
		}

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

		static Rectangle<T> Intersection(const Rectangle<T>& a, const Rectangle<T>& b) {

			Point2d<T> tl((std::max)(a.Left(), b.Left()), (std::max)(a.Top(), b.Top()));
			Point2d<T> br((std::min)(a.Right(), b.Right()), (std::min)(a.Bottom(), b.Bottom()));

			return Rectangle<T>(tl, br);

		}
		static Rectangle<T> Crop(const Rectangle<T>& rect, CropSide side, T amount) {

			switch (side) {

			case CropSide::Top:
				return Rectangle<T>(rect.X(), rect.Y() + amount, rect.Width(), rect.Height() - amount);
				break;

			case CropSide::Left:
				return Rectangle<T>(rect.X() + amount, rect.Y(), rect.Width() - amount, rect.Height());
				break;

			case CropSide::Right:
				return Rectangle<T>(rect.X(), rect.Y(), rect.Width() - amount, rect.Height());
				break;

			case CropSide::Bottom:
				return Rectangle<T>(rect.X(), rect.Y(), rect.Width(), rect.Height() - amount);
				break;

			}

			// If no valid crop side was provided, return the unmodified rectangle.
			return rect;

		}
		static Rectangle<T> Union(const Rectangle<T>& rect_a, const Rectangle<T>& rect_b) {

			Point2d<T> point_a(Math::Min(rect_a.Left(), rect_b.Left()), Math::Min(rect_a.Top(), rect_b.Top()));
			Point2d<T> point_b(Math::Max(rect_a.Right(), rect_b.Right()), Math::Max(rect_a.Bottom(), rect_b.Bottom()));

			return Rectangle<T>(point_a, point_b);

		}

		T Area() const {

			return Width() * Height();

		}

		template<typename U>
		operator Rectangle<U>() {

			return Rectangle<U>(static_cast<U>(X()), static_cast<U>(Y()), static_cast<U>(Width()), static_cast<U>(Height()));

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