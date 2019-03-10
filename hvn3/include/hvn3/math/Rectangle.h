#pragma once
#include "hvn3/core/Positionable2dBase.h"
#include "hvn3/core/SizeableBase.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Transform.h"
#include "hvn3/math/Line.h"

#include <array>

namespace hvn3 {

	enum class CropSide {
		Top,
		Left,
		Right,
		Bottom
	};

	template <typename T>
	class Rectangle :
		public Positionable2dBase<T>,
		public SizeableBase<T> {

		typedef Point2d<T> point_t;

	public:
		Rectangle() :
			Rectangle(0, 0) {}
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

		T X() const override {
			return Positionable2dBase<T>::X();
		}
		T Y() const override {
			return Positionable2dBase<T>::Y();
		}
		T X2() const {

			return X() + Width();

		}
		T Y2() const {

			return Y() + Height();

		}
		void SetX2(T value) {
			SetWidth(value - X());
		}
		void SetY2(T value) {
			SetHeight(value - Y());
		}

		T Width() const {
			return SizeableBase<T>::Width();
		}
		T Height() const {
			return SizeableBase<T>::Height();
		}

		T Bottom() const {

			return (std::max)(Y(), Y2());

		}
		T Top() const {

			return (std::min)(Y(), Y2());

		}
		T Left() const {

			return (std::min)(X(), X2());

		}
		T Right() const {

			return (std::max)(X(), X2());

		}
		void SetBottom(T value) {

			if (Y() > Y2())
				SetY(value);
			else
				SetY2(value);

		}
		void SetTop(T value) {

			if (Y() < Y2())
				SetY(value);
			else
				SetY2(value);

		}
		void SetLeft(T value) {

			if (X() < X2())
				SetX(value);
			else
				SetX2(value);

		}
		void SetRight(T value) {

			if (X() > X2())
				SetX(value);
			else
				SetX2(value);

		}
		Point2d<T> TopLeft() const {

			return Point2d<T>(Left(), Top());

		}
		Point2d<T> TopRight() const {

			return Point2d<T>(Right(), Top());

		}
		Point2d<T> BottomLeft() const {

			return Point2d<T>(Left(), Bottom());

		}
		Point2d<T> BottomRight() const {

			return Point2d<T>(Right(), Bottom());

		}
		Point2d<T> Midpoint() const {

			return Point2d<T>(X() + static_cast<T>(Width() / 2.0), Y() + static_cast<T>(Height() / 2.0));

		}
		Line<T> BottomEdge() const {
			return Line<T>(BottomLeft(), BottomRight());
		}
		Line<T> TopEdge() const {
			return Line<T>(TopLeft(), TopRight());
		}
		Line<T> LeftEdge() const {
			return Line<T>(TopLeft(), BottomLeft());
		}
		Line<T> RightEdge() const {
			return Line<T>(TopRight(), BottomRight());
		}

		void Translate(T x_offset, T y_offset) {

			Positionable2dBase<T>::SetPosition(X() + x_offset, Y() + y_offset);

		}
		void Translate(const Point2d<T>& offset) {
			Translate(offset.x, offset.y);
		}
		void Scale(T x_scale, T y_scale) {

			throw System::NotImplementedException();

		}
		// Returns an array of rotated points representing the vertices of the rectangle, ordered clockwise from the top-left vertex.
		std::array<point_t, 4> Rotate(const point_t& origin, T degrees) {

			Graphics::Transform transform;
			transform.Rotate(origin, degrees);

			return Transform(transform);

		}
		// Returns an array of transformed points representing the vertices of the rectangle, ordered clockwise from the top-left vertex.
		std::array<point_t, 4> Transform(const Graphics::Transform& transform) {

			point_t tl = transform.TransformPoint(TopLeft());
			point_t tr = transform.TransformPoint(TopRight());
			point_t br = transform.TransformPoint(BottomRight());
			point_t bl = transform.TransformPoint(BottomLeft());

			return { tl, tr, br, bl };

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

		class Scale ScaleTo(const Rectangle<T>& other) {
			return Scale(other.Width() / Width(), other.Height() / Height());
		}
		T Area() const {

			return Width() * Height();

		}

		bool ContainsPoint(const Point2d<T>& point) const {

			return (point.x >= X() && point.x < X2() && point.y >= Y() && point.y < Y2());

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