#pragma once
#include <cmath>
#include "Point2d.h"
#include "Utility.h"

namespace hvn3 {

	template <typename T>
	float PointDistanceSquared(const Point2d<T>& a, const Point2d<T>& b) {

		T dx = b.X() - a.X();
		T dy = b.Y() - a.Y();
		return dx * dx + dy * dy;

	}
	
	template <typename T>
	float PointDistance(const Point2d<T>& a, const Point2d<T>& b) {

		return std::sqrt(PointDistanceSquared(a, b));

	}

	template <typename T>
	Point2d<T> PointInDirection(const Point2d<T>& point, float degrees, float distance) {
		
		float rad = Math::DegreesToRadians(degrees);
		return Point2d<T>(point.X() + std::cos(rad) * distance, point.Y() - std::sin(rad) * distance);

	}

	template <typename T>
	float PointDirection(T ax, T ay, T bx, T by) {

		float angle = Math::RadiansToDegrees((std::atan2)(ay - by, bx - ax));
		if (angle < 0.0f) angle += 360.0f;

		return angle;

	}
	template <typename T>
	float PointDirection(const Point2d<T>& a, const Point2d<T>& b) {

		return PointDirection(a.X(), a.Y(), b.X(), b.Y());

	}

	template <typename T>
	void PointRotate(Point2d<T>& point, const Point2d<T>& origin, float angle) {

		float rad = Math::DegreesToRadians(angle);
		float s = std::sin(rad);
		float c = std::cos(rad);

		// Translate the Point to the origin.
		point -= origin;

		// Rotate the Point.
		float xnew = point.X() * c - point.Y() * s;
		float ynew = point.X() * s + point.Y() * c;

		// Translate the Point back.
		point.SetX(xnew + origin.X());
		point.SetY(ynew + origin.Y());

	}

}