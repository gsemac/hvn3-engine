#pragma once
#include "core/IUpdatable.h"
#include <vector>

namespace hvn3 {

	class IBroadPhase;
	class INarrowPhase;
	class ICollisionBody;
	struct CollisionManifold;

	class ICollisionManager : public IUpdatable {

	public:
		// Adds a new body to the collision manager.
		virtual void AddBody(ICollisionBody& body) = 0;
		// Marks a body for removal from the collision manager.
		virtual void RemoveBody(ICollisionBody& body) = 0;
		// Returns the broad phase collision manager used by the manager.
		virtual IBroadPhase& BroadPhase() = 0;
		// Returns the narrow phase collision manager used by the manager.
		virtual INarrowPhase& NarrowPhase() = 0;
		// Returns a collection of all colliding pairs found in the last update.
		virtual const std::vector<CollisionManifold>& CollidingPairs() const = 0;
		// Returns the number of collision bodies added to the manager.
		virtual size_t Count() const = 0;
		// Clears all collision bodies from the manager.
		virtual void Clear() = 0;

	};

}