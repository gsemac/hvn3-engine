#pragma once

#include "hvn3/collision/CohenSutherland.h"
#include "hvn3/collision/CollisionInfo.h"
#include "hvn3/collision/HitMask.h"
#include "hvn3/math/Rectangle.h"
#include "hvn3/math/Circle.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/math/Line.h"
#include "hvn3/math/MathUtils.h"

namespace hvn3 {

	// Tests for intersection between two rectangles, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Rectangle<T>& body1, const Rectangle<T>& body2) {

		return (body1.X() < body2.X2() && body1.X2() > body2.X() && body1.Y() < body2.Y2() && body1.Y2() > body2.Y());


	}

	// Tests for intersection between a circle and a line, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Circle<T>& body1, const Line<T>& body2) {

		// Note: This procedure uses the same logic as PointDistance, but avoids using the costly sqrt function.

		float _a = body1.X() - body2.First().X();
		float _b = body1.Y() - body2.First().Y();
		float _c = body2.Second().X() - body2.First().X();
		float _d = body2.Second().Y() - body2.First().Y();

		float dot = _a * _c + _b * _d;
		float len_sq = _c * _c + _d * _d;
		float param = -1.0f;
		if (len_sq != 0)
			param = dot / len_sq;

		float xx, yy;

		if (param < 0) {
			xx = body2.First().X();
			yy = body2.First().Y();
		}
		else if (param > 1) {
			xx = body2.Second().X();
			yy = body2.Second().Y();
		}
		else {
			xx = body2.First().X() + param * _c;
			yy = body2.First().Y() + param * _d;
		}

		float dx = body1.X() - xx;
		float dy = body1.Y() - yy;

		return (dx * dx + dy * dy) < (std::pow)(body1.Radius(), 2.0f);

		// return PointDistance(Point(a.X(), a.Y), b) <= a.Radius();

	}
	// Tests for intersection between a circle and a line, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Line<T>& body1, const Circle<T>& body2) {
		return TestIntersection(body2, body1);
	}

	// Tests for intersection between two lines, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Line<T>& body1, const Line<T>& body2) {

		T x1 = body1.First().X();
		T x2 = body1.Second().X();
		T x3 = body2.First().X();
		T x4 = body2.Second().X();
		T y1 = body1.First().Y();
		T y2 = body1.Second().Y();
		T y3 = body2.First().Y();
		T y4 = body2.Second().Y();

		T det = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		// If the determinant is 0, then there is no intersection.
		if (Math::IsZero(det))
			return false;

		// Calculate the (potential) point of intersection.
		T pre = (x1 * y2 - y1 * x2);
		T post = (x3 * y4 - y3 * x4);
		T x = (pre * (x3 - x4) - (x1 - x2) * post) / det;
		T y = (pre * (y3 - y4) - (y1 - y2) * post) / det;

		// Check that the point is on the lines.
		if (Math::IsLessThan(x, Math::Min(x1, x2)) ||
			Math::IsGreaterThan(x, Math::Max(x1, x2)) ||
			Math::IsLessThan(x, Math::Min(x3, x4)) ||
			Math::IsGreaterThan(x, Math::Max(x3, x4)))
			return false;
		if (Math::IsLessThan(y, Math::Min(y1, y2)) ||
			Math::IsGreaterThan(y, Math::Max(y1, y2)) ||
			Math::IsLessThan(y, Math::Min(y3, y4)) ||
			Math::IsGreaterThan(y, Math::Max(y3, y4)))
			return false;

		return true;

	}

	// Tests for intersection between a rectangle and a circle, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Rectangle<T>& body1, const Circle<T>& body2) {

		return body1.ContainsPoint(body2.Position()) ||
			TestIntersection(body2, Line<T>(body1.X(), body1.Y(), body1.X2(), body1.Y())) || // top
			TestIntersection(body2, Line<T>(body1.X(), body1.Y2(), body1.X2(), body1.Y2())) || // bottom 
			TestIntersection(body2, Line<T>(body1.X(), body1.Y(), body1.X(), body1.Y2())) || // left
			TestIntersection(body2, Line<T>(body1.X2(), body1.Y(), body1.X2(), body1.Y2())); // right

	}
	// Tests for intersection between a rectangle and a circle, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Circle<T>& body1, const Rectangle<T>& body2) {
		return TestIntersection(body2, body1);
	}

	// Tests for intersection between a rectangle and a line, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Rectangle<T>& body1, const Line<T>& body2) {

		// https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm

		auto out_codes = GetCohenSutherlandOutCodes(body1, body2);

		// One of the points is inside of the rectangle.
		if (out_codes.first == CohenSutherlandOutCode::Inside || out_codes.second == CohenSutherlandOutCode::Inside)
			return true;

		// Both points on one side of the rectangle (top, left, bottom, or right).
		if (static_cast<int>(out_codes.first & out_codes.second) != 0)
			return false;

		// Line passes through the rectangle (top/bottom or left/right).
		if (((out_codes.first | out_codes.second) == (CohenSutherlandOutCode::Left | CohenSutherlandOutCode::Right)) ||
			((out_codes.first | out_codes.second) == (CohenSutherlandOutCode::Top | CohenSutherlandOutCode::Bottom)))
			return true;

		// At this point, we know one point is to the left or the right, and one is to the top or the bottom.
		// We only need to test the two corresponding edges. 
		Line<T> edge1 = HasFlag(out_codes.first | out_codes.second, CohenSutherlandOutCode::Left) ? body1.LeftEdge() : body1.RightEdge();
		Line<T> edge2 = HasFlag(out_codes.first | out_codes.second, CohenSutherlandOutCode::Top) ? body1.TopEdge() : body1.BottomEdge();

		return TestIntersection(body2, edge1) || TestIntersection(body2, edge2);

	}
	// Tests for intersection between a rectangle and a line, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Line<T>& body1, const Rectangle<T>& body2) {
		return TestIntersection(body2, body1);
	}

	// Tests for intersection between two circles, and returns true if they are intersecting.
	template <typename T>
	bool TestIntersection(const Circle<T>& body1, const Circle<T>& body2) {

		return Math::Geometry::PointDistance(Point2d<T>(body1.X(), body1.Y()), Point2d<T>(body2.X(), body2.Y())) < (body1.Radius() + body2.Radius());

	}

	bool TestIntersection(HitMask& body1, HitMask& body2);
	bool TestIntersectionAt(HitMask& body1, const PointF& at1, HitMask& body2, const PointF& at2);

	CollisionInfo TestCollision(HitMask& body1, HitMask& body2);
	CollisionInfo TestCollisionAt(HitMask& body1, const PointF& at1, HitMask& body2, const PointF& at2);

	template<typename T>
	CollisionInfo TestCollision(const Circle<T>& body1, const Circle<T>& body2) {

		T centerDistance = Math::Geometry::Distance(body1.Position(), body2.Position());
		T radiiSum = body1.Radius() + body2.Radius();

		if (centerDistance > radiiSum)
			return CollisionInfo(false);

		if (centerDistance == 0.0f) {

			// The circles are centered on top of one another.
			// Just pick a direction and radius to use.

			return CollisionInfo(Vector2d(1.0f, 0.0f), body1.Radius());

		}
		else {

			Vector2d normal = Vector2d(body1.Position(), body2.Position()) / centerDistance; // unit vector
			float penetrationDepth = radiiSum - centerDistance;

			return CollisionInfo(normal, penetrationDepth);

		}

	}

	template<typename T>
	Rectangle<T> CalculateAabb(const Rectangle<T>& body) {
		return body;
	}
	template<typename T>
	Rectangle<T> CalculateAabb(const Line<T>& body) {
		return RectangleF(body.First(), body.Second());
	}
	template<typename T>
	Rectangle<T> CalculateAabb(const Circle<T>& body) {

		return RectangleF(
			PointF(body.X() - body.Radius(), body.Y() - body.Radius()),
			PointF(body.X() + body.Radius(), body.Y() + body.Radius())
		);

	}

	// Tests for a collision between two rectangles. If they are colliding, returns true and sets the collision manifold information. Otherwise, returns false.
	template<typename T>
	bool ResolveCollision(const Rectangle<T>& hitmask_1, const Rectangle<T>& hitmask_2, CollisionResult& manifold) {

		// Vector from A to B.			
		Vector2d normal(hitmask_1.Midpoint(), hitmask_2.Midpoint());

		// Calculate half extents along the x-axis for each mask.
		T a_extent = hitmask_1.Width() / static_cast<T>(2);
		T b_extent = hitmask_2.Width() / static_cast<T>(2);

		// Calculate overlap on the x-axis.
		T x_overlap = (a_extent + b_extent) - Math::Abs(normal.X());

		// SAT test on the x-axis.
		if (x_overlap > static_cast<T>(0)) {

			// Calculate half extents along the y-axis for each mask.
			a_extent = hitmask_1.Height() / static_cast<T>(2);
			b_extent = hitmask_2.Height() / static_cast<T>(2);

			// Calculate overlap on the y-axis.
			T y_overlap = (a_extent + b_extent) - Math::Abs(normal.Y());

			// SAT test on the x-axis.
			if (y_overlap > static_cast<T>(0)) {

				// Find out which axis is the axis of least penetration.
				if (x_overlap < y_overlap) {

					if (normal.X() < static_cast<T>(0))
						manifold.normal = Vector2d(-1.0f, 0.0f);
					else
						manifold.normal = Vector2d(1.0f, 0.0f);

					manifold.penetrationDepth = x_overlap;
					return true;

				}
				else {

					if (normal.Y() < static_cast<T>(0))
						manifold.normal = Vector2d(0.0f, -1.0f);
					else
						manifold.normal = Vector2d(0.0f, 1.0f);

					manifold.penetrationDepth = y_overlap;
					return true;

				}
			}
		}

		// No collision detected.
		return false;

	}

	template <typename T>
	// Tests for a collision between two rectangles. If they are colliding, returns true and sets the collision manifold information, taking into account the direction of movement. Otherwise, returns false.
	bool ResolveCollision(const Rectangle<T>& hitmask_1, const Rectangle<T>& hitmask_2, T direction, CollisionResult& manifold) {

		throw NotImplementedException();

		//// Resolve the collision without consideration for direction first, which will generate the face normal.
		//if (!(ResolveCollision(hitmask_1, hitmask_2, manifold)))
		//	return false;

		//T normal = Math::NormalizeAngle(direction + static_cast<T>(180)); // Direction 0-360
		//T theta = Math::NormalizeAngle(normal, static_cast<T>(0), static_cast<T>(90)); // Direction 0-90
		//int quadrant = Math::AngleQuadrant(normal);

		//T adjacent;
		////if (quadrant == 1 || quadrant == 3)
		//	// Use horizontal difference for the adjacent side.
		////	adjacent = hitmask_1.Right() > hitmask_2.Left() ? hitmask_1.Right() - hitmask_2.Left() : hitmask_1.Left() - hitmask_2.Right();
		////else
		//	// Use vertical difference for the adjacent side.
		//	adjacent = hitmask_1.Bottom() > hitmask_2.Top() ? hitmask_1.Bottom() - hitmask_2.Top() : hitmask_1.Top() - hitmask_2.Bottom();

		//// Calculate the penetration depth.
		//manifold.PenetrationDepth = adjacent / Math::Cosine(theta);
		//manifold.Normal = Vector2d::FromDirection(normal, 1.0f);

		//// Since there was indeed a collision, return true.
		//return true;

	}

	// Tests for a collision between a circle and a line. If they are colliding, returns true and sets the collision manifold information relative to the circle. Otherwise, returns false.
	template <typename T>
	bool ResolveCollision(const Circle<T>& body_1, const Line<T> body_2, CollisionResult& manifold) {

		throw NotImplementedException();

	}

	template<typename ValueType>
	bool ResolveCollision(const Line<ValueType>& mask1, const Line<ValueType>& mask2, CollisionResult& manifold) {

		auto result = Math::Geometry::GetIntersectionPoints(mask1, mask2);

		if (result)
			manifold.position = result.points[0];

		return static_cast<bool>(result);

	}
	template<typename ValueType>
	bool ResolveCollision(const Line<ValueType>& mask1, const Rectangle<ValueType>& mask2, CollisionResult& manifold) {

		// Check if the line is entirely inside of the rectangle. If so, trivially accept.

		auto out_codes = GetCohenSutherlandOutCodes(mask2, mask1);

		if (static_cast<int>(out_codes.first | out_codes.second) == 0) {

			manifold.position = mask1.Midpoint();

			return true;

		}

		// Otherwise, check for an edge intersection.

		auto result = Math::Geometry::GetIntersectionPoints(mask1, mask2);

		if (result) {

			if (result.infinite)
				manifold.position = mask1.Midpoint();
			else
				manifold.position = result.points[0];

			return true;

		}

		return false;

	}
	template<typename ValueType>
	bool ResolveCollision(const Rectangle<ValueType>& mask1, const Line<ValueType>& mask2, CollisionResult& manifold) {
		return ResolveCollision(mask2, mask1, manifold);
	}

}