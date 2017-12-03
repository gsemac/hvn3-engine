#include "hvn3/collision/CollisionBodyMutator.h"
#include "hvn3/collision/ICollisionBody.h"

namespace hvn3 {
	namespace System {

		CollisionBodyMutator::CollisionBodyMutator(ICollisionBody& body) : 
			_body(body) {}

		void CollisionBodyMutator::SetManager(ICollisionManager* manager) {

			_body._setManager(manager);

		}

	}
}