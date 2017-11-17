#include "collision/CollisionBodyMutator.h"
#include "collision/ICollisionBody.h"

namespace hvn3 {
	namespace System {

		CollisionBodyMutator::CollisionBodyMutator(ICollisionBody& body) : 
			_body(body) {}

		void CollisionBodyMutator::SetPhasePair(IBroadPhase* broad_phase, INarrowPhase* narrow_phase) {

			_body._setCollisionPhasePair(broad_phase, narrow_phase);

		}

	}
}