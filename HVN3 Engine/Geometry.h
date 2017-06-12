#pragma once
#include "Rectangle.h"
#include "Circle.h"
#include "Line.h"

namespace hvn3 {
	
	template <typename T>
	bool PointIn(const Point2d<T>& point, const Rectangle<T>& rect) {

		return (point.X() >= rect.X() && point.X() < rect.X2() && point.Y() >= rect.Y() && point.Y() < rect.Y2());

	}

	template <typename T>
	float PointDistance(const Point2d<T>& point, const Line<T>& line) {

		float a = point.X() - line.First().X();
		float b = point.Y() - line.First().Y();
		float c = line.Second().X() - line.First().X();
		float d = line.Second().Y() - line.First().Y();

		float dot = a * c + b * d;
		float len_sq = c * c + d * d;
		float param = -1.0f;
		if (len_sq != 0)
			param = dot / len_sq;

		float xx, yy;

		if (param < 0) {
			xx = line.First().X();
			yy = line.First().Y();
		}
		else if (param > 1) {
			xx = line.Second().X();
			yy = line.Second().Y();
		}
		else {
			xx = line.First().X() + param * c;
			yy = line.First().Y() + param * d;
		}

		float dx = point.X() - xx;
		float dy = point.Y() - yy;

		return std::sqrt(dx * dx + dy * dy);

	}
	template <typename T>
	float PointDistance(const Point2d<T>& point, const Rectangle<T>& rect) {

		return (std::sqrtf)(PointDistanceSquared(point, rect));

	}
	
	template <typename T>
	float PointDistanceSquared(const Point2d<T>& point, const Rectangle<T>& rect) {

		float dx = (std::max)((std::abs)(point.X() - rect.X() + rect.Width() / 2.0f) - rect.Width() / 2.0f, 0.0f);
		float dy = (std::max)((std::abs)(point.Y() - rect.Y() + rect.Height() / 2.0f) - rect.Height() / 2.0f, 0.0f);

		return dx * dx + dy * dy;

	}
	
	template <typename T>
	IPositionable2d<T>& Translate(IPositionable2d<T>& obj, T x_offset, T y_offset) {

		obj.SetX(obj.X() + x_offset);
		obj.SetY(obj.Y() + y_offset);

		return obj;

	}

	template <typename T>
	float Distance(const Rectangle<T>& a, const Rectangle<T>& b) {

		// If the two rectangles intersect, the distance between them is zero.
		if (Intersects(a, b)) return 0.0f;

		// Otherwise, the distance is the distance to the closest corner of the other rectangle.
		bool left = b.Right() < a.Left(); // b is on the left of a
		bool right = b.Left() > a.Right(); // b is on the right of a
		bool bottom = b.Top() > a.Bottom(); // b is below a
		bool top = b.Bottom() < a.Top(); // b is above a

		if (top && left)
			return PointDistance(b.BottomRight(), a.TopLeft());
		else if (left && bottom)
			return PointDistance(b.TopRight(), a.BottomLeft());
		else if (bottom && right)
			return PointDistance(b.TopLeft(), a.BottomRight());
		else if (right && top)
			return PointDistance(b.BottomLeft(), a.TopRight());
		else if (left)
			return a.Left() - b.Right();
		else if (right)
			return b.Left() - a.Right();
		else if (bottom)
			return b.Top() - a.Bottom();
		else
			return a.Top() - b.Bottom();

	}

	template <typename T>
	bool Intersects(const Rectangle<T>& a, const Rectangle<T>& b) {

		return (a.X() < b.X2() && a.X2() > b.X() && a.Y() < b.Y2() && a.Y2() > b.Y());

	}
	template <typename T>
	bool Intersects(const Circle<T>& a, const Line<T>& b) {

		// Note: This procedure uses the same logic as PointDistance, but avoids using the costly sqrt function.

		float _a = a.X() - b.First().X();
		float _b = a.Y() - b.First().Y();
		float _c = b.Second().X() - b.First().X();
		float _d = b.Second().Y() - b.First().Y();

		float dot = _a * _c + _b * _d;
		float len_sq = _c * _c + _d * _d;
		float param = -1.0f;
		if (len_sq != 0)
			param = dot / len_sq;

		float xx, yy;

		if (param < 0) {
			xx = b.First().X();
			yy = b.First().Y();
		}
		else if (param > 1) {
			xx = b.Second().X();
			yy = b.Second().Y();
		}
		else {
			xx = b.First().X() + param * _c;
			yy = b.First().Y() + param * _d;
		}

		float dx = a.X() - xx;
		float dy = a.Y() - yy;

		return (dx * dx + dy * dy) < (std::pow)(a.Radius(), 2.0f);

		// return PointDistance(Point(a.X(), a.Y), b) <= a.Radius();

	}
	template <typename T>
	bool Intersects(const Line<T>& a, const Line<T>& b) {

		return false;

	}
	template <typename T>
	bool Intersects(const Rectangle<T>& a, const Circle<T>& b) {

		return PointIn(Point2d<T>(b.X(), b.Y()), a) ||
			Intersects(b, Line<T>(a.X(), a.Y(), a.X2(), a.Y())) || // top
			Intersects(b, Line<T>(a.X(), a.Y2(), a.X2(), a.Y2())) || // bottom 
			Intersects(b, Line<T>(a.X(), a.Y(), a.X(), a.Y2())) || // left
			Intersects(b, Line<T>(a.X2(), a.Y(), a.X2(), a.Y2())); // right

	}
	template <typename T>
	bool Intersects(const Rectangle<T>& a, const Line<T>& b) {

		return false;

	}
	template <typename T>
	bool Intersects(const Circle<T>& a, const Circle<T>& b) {

		return PointDistance(Point(a.X(), a.Y()), Point(b.X(), b.Y())) < (a.Radius() + b.Radius());

	}

}