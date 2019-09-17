#pragma once
#include "hvn3/core/Positionable2dBase.h"
#include "hvn3/core/SizeableBase.h"
#include "hvn3/exceptions/Exception.h"
#include "hvn3/graphics/Transform.h"
#include "hvn3/math/Line.h"
#include "hvn3/utility/Scale.h"

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
		typedef Line<T> line_t;
		typedef Rectangle<T> rectangle_t;

	public:
		Rectangle() :
			Rectangle(0, 0) {
		}
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
		point_t TopLeft() const {

			return Point2d<T>(Left(), Top());

		}
		point_t TopRight() const {

			return point_t(Right(), Top());

		}
		point_t BottomLeft() const {

			return point_t(Left(), Bottom());

		}
		point_t BottomRight() const {

			return point_t(Right(), Bottom());

		}
		point_t Midpoint() const {

			return point_t(X() + static_cast<T>(Width() / 2.0), Y() + static_cast<T>(Height() / 2.0));

		}
		line_t BottomEdge() const {
			return line_t(BottomLeft(), BottomRight());
		}
		line_t TopEdge() const {
			return line_t(TopLeft(), TopRight());
		}
		line_t LeftEdge() const {
			return line_t(TopLeft(), BottomLeft());
		}
		line_t RightEdge() const {
			return line_t(TopRight(), BottomRight());
		}

		rectangle_t Translate(T offsetX, T offsetY) const {
			return rectangle_t(X() + offsetX, Y() + offsetY, Width(), Height());
		}
		rectangle_t Translate(const point_t& offset) const {
			return Translate(offset.x, offset.y);
		}

		template<typename U>
		rectangle_t Scale(U scaleX, U scaleY) const {

			T width = static_cast<T>(static_cast<U>(Width()) * x_scale);
			T height = static_cast<T>(static_cast<U>(Height()) * y_scale);

			return rectangle_t(X(), Y(), width, height);

		}
		rectangle_t Scale(const hvn3::Scale& scale) {
			return Scale(scale.XScale(), scale.YScale());
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

		rectangle_t Intersection(const rectangle_t& other) const {

			point_t tl((std::max)(Left(), other.Left()), (std::max)(Top(), other.Top()));
			point_t br((std::min)(Right(), other.Right()), (std::min)(Bottom(), other.Bottom()));

			return rectangle_t(tl, br);

		}
		rectangle_t Crop(CropSide side, T amount) const {

			switch (side) {

			case CropSide::Top:
				return rectangle_t(X(), Y() + amount, Width(), Height() - amount);
				break;

			case CropSide::Left:
				return rectangle_t(X() + amount, Y(), Width() - amount, Height());
				break;

			case CropSide::Right:
				return rectangle_t(X(), Y(), Width() - amount, Height());
				break;

			case CropSide::Bottom:
				return rectangle_t(X(), Y(), Width(), Height() - amount);
				break;

			}

			// If no valid crop side was provided, return the unmodified rectangle.
			return rectangle_t(*this);

		}
		rectangle_t Union(const rectangle_t& other) const {

			point_t point_a(Math::Min(Left(), other.Left()), Math::Min(Top(), other.Top()));
			point_t point_b(Math::Max(Right(), other.Right()), Math::Max(Bottom(), other.Bottom()));

			return rectangle_t(point_a, point_b);

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

		bool Intersects(const Rectangle<T>& other) {
			return X() < other.X2() && X2() > other.X() && Y() < other.Y2() && Y2() > other.Y();
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