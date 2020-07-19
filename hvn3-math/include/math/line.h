#pragma once

#include "math/math.h"
#include "math/point.h"
#include "math/rectangle.h"

#include <cmath>

namespace hvn3::math {

	template<typename T>
	class Line2 {

	public:
		Line2(T startX, T startY, T endX, T endY) {

			start = Point2<T>(startX, startY);
			end = Point2<T>(endX, endY);

		}
		Line2(const Point2<T>& start, const Point2<T>& end) {

			this->start = start;
			this->end = end;

		}

		const Point2<T>& Start() const {

			return start;

		}
		const Point2<T>& End() const {

			return end;

		}

		void SetStart(const Point2<T>& value) {

			start = value;

		}
		void SetEnd(const Point2<T>& value) {

			end = value;

		}

		T X() const {

			return start.X();

		}
		T Y() const {

			return start.Y();

		}
		T X2() const {

			return end.X();

		}
		T Y2() const {

			return end.Y();

		}

		void SetX(T value) {

			SetStart(Point2<T>(value, start.Y()));

		}
		void SetY(T value) {

			SetStart(Point2<T>(start.X(), value));

		}
		void SetX2(T value) {

			SetEnd(Point2<T>(value, start.Y()));

		}
		void SetY2(T value) {

			SetEnd(Point2<T>(start.X(), value));

		}

		Point2<T> Midpoint() const {

			float midpointX = (start.X() + end.X()) / static_cast<T>(2);
			float midpointY = (start.Y() + end.Y()) / static_cast<T>(2);

			return Point2<T>(midpointX, midpointY);

		}

		T Length() const {

			return std::sqrt(SquaredLength());

		}
		T SquaredLength() const {

			return start.SquaredDistanceTo(end);

		}

		bool ContainsPoint(const Point2<T>& point) const {

			// If the point is on the line, then the sum of its distance from the start and end points is equal to the total length of the line.
			// This should hold even if the distances are squared, which makes the calculation more efficient.

			T startToPointDistSquared = start.SquaredDistanceTo(point);
			T pointToEndDistSquared = point.SquaredDistanceTo(end);
			T distSquared = SquaredLength();

			return math::AreEqual(distSquared, startToPointDistSquared + pointToEndDistSquared);

		}

		T Slope() const {

			return DeltaY() / DeltaX();

		}
		T Incline() const {

			return std::abs(Slope());

		}

		T InterceptX() const {

			return -InterceptY() / Slope();

		}
		T InterceptY() const {

			return start.Y() - (Slope() * start.X());

		}
		T DeltaX() const {

			return end.X() - start.X();

		}
		T DeltaY() const {

			end.Y() - start.Y();

		}

		Rectangle<T> Bounds() const {

			return Rectangle<T>(X(), Y(), DeltaX(), DeltaY());

		}

		Line2<T> Translate(T offsetX, T offsetY) const {

			return Line2<T>(X() + offsetX, Y() + offsetY, X2() + offsetX, Y2() + offsetY);

		}
		Line2<T> Translate(const Point2<T>& offset) const {

			return Translate(offset.X(), offset.Y());

		}

		Line2<T>& operator=(const Line2<T>& rhs) {

			SetStart(rhs.Start());
			SetEnd(rhs.End());

			return *this;

		}

	private:
		Point2<T> start;
		Point2<T> end;

	};

	using Line2i = Line2<int>;
	using Line2f = Line2<float>;

}