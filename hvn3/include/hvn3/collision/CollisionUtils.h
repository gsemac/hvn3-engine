#pragma once
#include "hvn3/math/Rectangle.h"
#include "hvn3/math/Circle.h"
#include "hvn3/math/Line.h"
#include "hvn3/collision/CollisionResult.h"
#include "hvn3/math/MathUtils.h"

namespace hvn3 {

	// #todo Move all "TestIntersection"-etc. functions into here?

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

		throw System::NotImplementedException();

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

		throw System::NotImplementedException();

	}

}