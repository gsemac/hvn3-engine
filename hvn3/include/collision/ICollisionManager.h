#pragma once
#include "core/IUpdatable.h"

namespace hvn3 {

	class IBroadPhase;
	class INarrowPhase;
	class ICollisionBody;

	class ICollisionManager : public IUpdatable {

	public:
		// Returns the broad phase collision manager used by the manager.
		virtual IBroadPhase& BroadPhase() = 0;
		// Returns the narrow phase collision manager used by the manager.
		virtual INarrowPhase& NarrowPhase() = 0;
		// Returns a collection of all colliding pairs found in the last update.
		virtual const std::vector<CollisionManifold>& CollidingPairs() const = 0;
		// Clears all collision bodies from the manager.
		virtual void Clear() = 0;

	};

}