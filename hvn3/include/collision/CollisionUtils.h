#pragma once
#include "math/Rectangle.h"
#include "math/Circle.h"
#include "math/Line.h"
#include "collision/CollisionManifold.h"
#include "math/MathUtils.h"

namespace hvn3 {
	namespace Collision {

		// Tests for a collision between two rectangles. If they are colliding, returns true and sets the collision manifold information. Otherwise, returns false.
		template<typename T>
		bool TestCollision(const Rectangle<T>& body_1, const Rectangle<T>& body_2, CollisionManifold& manifold) {

			// Vector from A to B.			
			Vector2d normal(body_1.Midpoint(), body_2.Midpoint());

			// Calculate half extents along the x-axis for each mask.
			T a_extent = body_1.Width() / static_cast<T>(2);
			T b_extent = body_2.Width() / static_cast<T>(2);

			// Calculate overlap on the x-axis.
			T x_overlap = (a_extent + b_extent) - Math::Abs(normal.X());
			
			// SAT test on the x-axis.
			if (x_overlap > static_cast<T>(0)) {

				// Calculate half extents along the y-axis for each mask.
				a_extent = body_1.Height() / static_cast<T>(2);
				b_extent = body_2.Height() / static_cast<T>(2);

				// Calculate overlap on the y-axis.
				T y_overlap = (a_extent + b_extent) - Math::Abs(normal.Y());
	
				// SAT test on the x-axis.
				if (y_overlap > static_cast<T>(0)) {
					
					// Find out which axis is the axis of least penetration.
					if (x_overlap < y_overlap) {
					
						if (normal.X() < static_cast<T>(0))
							manifold.Normal = Vector2d(-1.0f, 0.0f);
						else
							manifold.Normal = Vector2d(1.0f, 0.0f);

						manifold.Penetration = x_overlap;
						return true;

					}
					else {
						
						if (normal.Y() < static_cast<T>(0))
							manifold.Normal = Vector2d(0.0f, -1.0f);
						else
							manifold.Normal = Vector2d(0.0f, 1.0f);

						manifold.Penetration = y_overlap;
						return true;

					}
				}
			}
			
			// No collision detected.
			return false;

		}

		// Tests for a collision between a circle and a line. If they are colliding, returns true and sets the collision manifold information relative to the circle. Otherwise, returns false.
		template <typename T>
		bool TestCollision(const Circle<T>& body_1, const Line<T> body_2, CollisionManifold& manifold) {

			return false;

		}

	}
}