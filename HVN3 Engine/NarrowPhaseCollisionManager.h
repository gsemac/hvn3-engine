#pragma once
#include "Geometry.h"

namespace hvn3 {

	class Collider;
	class Sprite;
	class SpriteMask;

	class NarrowPhaseCollisionManager {

	public:
		bool TestCollision(Collider* a, Collider* b) const;
		bool TestCollision(Collider* a, const PointF& position_a, Collider* b, const PointF& position_b) const;

		// Returns true if two axis-aligned Sprites intersect at the specified positions.
		bool TestIntersection(Sprite* s1, Sprite* s2, const PointF& p1, const PointF& p2) const;
		// Returns true if the axis-aligned Sprite intersects with the Circle at the specific position.
		bool TestIntersection(Sprite* sprite, const CircleF& circle, const PointF& pos) const;
		// Returns true if the SpriteMask intersects with the Circle at the specified position.
		bool TestIntersection(const SpriteMask& mask, const CircleF& circle, const PointF& pos) const;

	};

}