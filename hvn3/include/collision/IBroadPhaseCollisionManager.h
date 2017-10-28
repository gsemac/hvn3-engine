#pragma once
#include <vector>
#include "math/Rectangle.h"
#include "math/Line.h"
#include "core/IUpdatable.h"

namespace hvn3 {
	namespace Collision {

		class ICollisionBody;

		struct RayCastResult {
			ICollisionBody* Hit;
			PointF HitPosition;
		};

		class IBroadPhaseCollisionManager : public IUpdatable {

		public:
			typedef ICollisionBody collider_type;
			typedef std::vector<std::pair<collider_type*, collider_type*>> collider_pair_collection_type;
			typedef std::vector<collider_type*> collider_collection_type;

			virtual void AddBody(ICollisionBody* body) = 0;
			virtual bool RemoveBody(ICollisionBody* body) = 0;
			virtual void ClearAll() = 0;

			// Returns a vector of potentially-colliding pairs.
			virtual const collider_pair_collection_type& FindCandidatePairs() = 0;
			// Returns the first collider that collides with the given point, or null if none.
			virtual ICollisionBody* Pick(const PointF& point) const = 0;
			// Generates a vector of colliders that collide with the given region.
			virtual void QueryRegion(const RectangleF& region, collider_collection_type& output, int filter = 0) const = 0;
			// Returns the nearest collider to the given point, or nullptr if no colliders found.
			virtual ICollisionBody* QueryNearest(const PointF& point, int filter = 0) const = 0;
			// Returns the first collider that the ray hits, or null if none.
			virtual RayCastResult RayCast(const LineF& ray) const = 0;

		};

	}
}