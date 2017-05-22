#pragma once
#include "Geometry.h"

namespace hvn3 {

	class ICollidable;
	class Sprite;
	class SpriteMask;

	class NarrowPhaseCollisionManager {

	public:
		static bool TestCollision(ICollidable* a, ICollidable* b);
		static bool TestCollision(ICollidable* a, float ax, float ay, ICollidable* b, float bx, float by);

		// Returns true if two axis-aligned Sprites intersect at the specified positions.
		static bool TestIntersection(Sprite* s1, Sprite* s2, const Point& p1, const Point& p2);
		// Returns true if the axis-aligned Sprite intersects with the Circle at the specific position.
		static bool TestIntersection(Sprite* sprite, const Circle& circle, const Point& pos);
		// Returns true if the SpriteMask intersects with the Circle at the specified position.
		static bool TestIntersection(const SpriteMask& mask, const Circle& circle, const Point& pos);

	};

}