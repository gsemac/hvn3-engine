#pragma once

#include "math/point.h"

#include <cmath>

namespace hvn3::math {

	template<typename T>
	struct Rectangle {

		T x;
		T y;
		T width;
		T height;

		Rectangle(T x, T y, T width, T height) {

			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;

		}
		Rectangle(T width, T height) :
			Rectangle(static_cast<T>(0), static_cast<T>(0), width, height) {
		}
		Rectangle(const Rectangle<T>& other) :
			Rectangle(other.X(), other.Y(), other.Width(), other.Height()) {
		}

		T X() const {

			return x;

		}
		T Y() const {

			return y;

		}
		T X2() const {

			return X() + Width();

		}
		T Y2() const {

			return Y() + Height();

		}
		T Width() const {

			return width;

		}
		T Height() const {

			return height;

		}

		void SetX(T value) {

			x = value;

		}
		void SetY(T value) {

			y = value;

		}
		void SetXY(T x, T y) {

			SetX(x);
			SetY(y);

		}
		void SetX2(T value) {

			SetWidth(value - X());

		}
		void SetY2(T value) {

			SetHeight(value - Y());

		}
		void SetWidth(T value) {

			width = value;

		}
		void SetHeight(T value) {

			height = value;

		}

		T Top() const {

			return std::min(Y(), Y2());

		}
		T Left() const {

			return std::min(X(), X2());

		}
		T Bottom() const {

			return std::max(Y(), Y2());

		}
		T Right() const {

			return std::max(X(), X2());

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
		void SetBottom(T value) {

			if (Y() > Y2())
				SetY(value);
			else
				SetY2(value);

		}
		void SetRight(T value) {

			if (X() > X2())
				SetX(value);
			else
				SetX2(value);

		}

		bool ContainsPoint(const Point2<T>& point) const {

			return point.X() >= X() &&
				point.X() < X2() &&
				point.Y() >= Y() &&
				point.Y() < Y2();

		}
		bool IntersectsWith(const Rectangle<T>& other) const {

			return X() < other.X2() &&
				X2() > other.X() &&
				Y() < other.Y2() &&
				Y2() > other.Y();

		}

		Rectangle<T> Translate(T offsetX, T offsetY) const {

			return Rectangle<T>(X() + offsetX, Y() + offsetY, Width(), Height());

		}
		Rectangle<T> Translate(const Point2<T>& offset) const {

			return Translate(offset.X(), offset.Y());

		}
		template<typename U>
		Rectangle<T> Scale(const U& xFactor, const U& yFactor) const {

			return Rectangle<T>(X(), Y(), static_cast<T>(Width() * xFactor), static_cast<T>(Width() * yFactor));

		}
		template<typename U>
		Rectangle<T> Scale(const U& factor) const {

			return Scale(factor, factor);

		}

		Rectangle<T>& operator=(const Rectangle<T>& rhs) {

			SetX(rhs.X());
			SetY(rhs.Y());
			SetWidth(rhs.Width());
			SetHeight(rhs.Height());

			return *this;

		}

		template<typename U>
		Rectangle<T>& operator*=(const U& factor) {

			SetWidth(Width() * static_cast<T>(factor));
			SetHeight(Height() * static_cast<T>(factor));

			return *this;

		}
		template<typename U>
		Rectangle<T>& operator/=(const U& factor) {

			SetWidth(Width() / static_cast<T>(factor));
			SetHeight(Height() / static_cast<T>(factor));

			return *this;

		}

	};

	using RectangleF = Rectangle<float>;
	using RectangleI = Rectangle<int>;

}