#pragma once
#include "hvn3/collision/CollisionDefs.h"
#include "hvn3/core/IUpdatable.h"
#include "hvn3/math/Line.h"
#include "hvn3/math/Rectangle.h"
#include <vector>

namespace hvn3 {

	class ICollider;

	struct RayCastResult {
		
		RayCastResult() {
			hit = nullptr;
		}

		ICollider* hit;
		PointF hitPosition;

	};

	class IBroadPhase :
		public IUpdatable {

	public:
		typedef std::vector<ICollider*> collider_vector_type;
		typedef std::pair<ICollider*, ICollider*> collider_pair_type;
		typedef std::vector<collider_pair_type> collider_pair_vector_type;

		// Adds the given body to the broad phase system. The system does not assume ownership of the body.
		virtual void Add(const IColliderPtr& body) = 0;
		// Returns the number of collision bodies in the system.
		virtual size_t Count() const = 0;
		// Removes all bodies from the system.
		virtual void Clear() = 0;

		// Returns a vector of potentially-colliding pairs.
		virtual const collider_pair_vector_type& FindCandidatePairs() = 0;
		// Generates a vector of colliders that collide with the given region.
		virtual void QueryRegion(const RectangleF& region, collider_vector_type& output, int filter = 0) const = 0;
		//virtual void QueryLine(const LineF& line, collider_vector_type& output, int filter = 0) const = 0;
		// Returns the nearest collider to the given point, or nullptr if no colliders found.
		virtual ICollider* QueryNearest(const PointF& point, int filter = 0) const = 0;
		// Returns the first collider that the ray hits, or null if none.
		virtual RayCastResult RayCast(const LineF& ray, int mask = 0) const = 0;

		// Returns the first collider that collides with the given point.
		virtual CollisionManifold Pick(const PointF& point) const = 0;

	};

}