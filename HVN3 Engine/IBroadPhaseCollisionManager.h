#pragma once
#include <vector>
#include "Rectangle.h"
#include "Line.h"

namespace hvn3 {

	class Collider;

	struct RayCastResult {
		Collider* Hit;
		PointF HitPosition;
	};

	class IBroadPhaseCollisionManager {
		
	public:
		typedef Collider* ColliderType;
		typedef std::vector<std::pair<ColliderType, ColliderType>> ColliderPairCollection;
		typedef std::vector<ColliderType> ColliderCollection;

		// Adds a new collider to the broadphase.
		virtual void AddCollider(ColliderType collider) = 0;
		// Removes a collider from the broadphase.
		virtual void RemoveCollider(ColliderType collider) = 0;
		// Clears all colliders from the broadphase.
		virtual void ClearColliders() = 0;

		// Updates broadphase to react to collider state changes.
		virtual void Update() = 0;

		// Returns a vector of potentially-colliding pairs.
		virtual const ColliderPairCollection& FindPairs() = 0;
		// Returns the first collider that collides with the given point, or null if none.
		virtual ColliderType Pick(const PointF& point) const = 0;
		// Generates a vector of colliders that collide with the given region.
		virtual void QueryRegion(const RectangleF& region, ColliderCollection& output, int filter = 0) const = 0;
		// Returns the nearest collider to the given point, or nullptr if no colliders found.
		virtual ColliderType QueryNearest(const PointF& point, int filter = 0) const = 0;
		// Returns the first collider that the ray hits, or null if none.
		virtual RayCastResult RayCast(const LineF& ray) const = 0;

	};

}