#pragma once
#include "math/GeometryUtils.h"
#include "collision/CollisionManifold.h"

namespace hvn3 {
	
	class Sprite;

	namespace Collision {

		class NarrowPhaseCollisionManager {

		public:
			bool TestCollision(ICollisionBody* a, ICollisionBody* b, CollisionManifold& m) const;
			bool TestCollision(ICollisionBody* a, const PointF& position_a, ICollisionBody* b, const PointF& position_b, CollisionManifold& m) const;

			// Returns true if two axis-aligned Sprites intersect at the specified positions.
			bool TestIntersection(Sprite* s1, Sprite* s2, const PointF& p1, const PointF& p2) const;
			// Returns true if the axis-aligned Sprite intersects with the Circle at the specific position.
			bool TestIntersection(Sprite* sprite, const CircleF& circle, const PointF& pos) const;
			// Returns true if the SpriteMask intersects with the Circle at the specified position.
			//bool TestIntersection(const SpriteMask& mask, const CircleF& circle, const PointF& pos) const;

		};

	}
}