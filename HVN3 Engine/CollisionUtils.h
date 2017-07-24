#pragma once
#include "Rectangle.h"
#include "CollisionManifold.h"
#include "MathUtils.h"

namespace hvn3 {
	namespace Collision {

		template<typename T>
		bool TestCollision(const Rectangle<T>& mask_a, const Rectangle<T>& mask_b, CollisionManifold& manifold) {

			// Vector from A to B.			
			Vector2d normal(mask_a.Midpoint(), mask_b.Midpoint());

			// Calculate half extents along the x-axis for each mask.
			float a_extent = mask_a.Width() / 2.0f;
			float b_extent = mask_b.Width() / 2.0f;

			// Calculate overlap on the x-axis.
			float x_overlap = (a_extent + b_extent) - Math::Abs(normal.X());
			
			// SAT test on the x-axis.
			if (x_overlap > 0.0f) {

				// Calculate half extents along the y-axis for each mask.
				a_extent = mask_a.Height() / 2.0f;
				b_extent = mask_b.Height() / 2.0f;

				// Calculate overlap on the y-axis.
				float y_overlap = (a_extent + b_extent) - Math::Abs(normal.Y());
	
				// SAT test on the x-axis.
				if (y_overlap > 0.0f) {
					
					// Find out which axis is the axis of least penetration.
					if (x_overlap < y_overlap) {
					
						if (normal.X() < 0.0f)
							manifold.Normal = Vector2d(-1.0f, 0.0f);
						else
							manifold.Normal = Vector2d(1.0f, 0.0f);

						manifold.Penetration = x_overlap;
						return true;

					}
					else {
						
						if (normal.Y() < 0.0f)
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

	}
}