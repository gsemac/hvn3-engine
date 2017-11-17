#include "collision/CollisionBodyMutator.h"
#include "collision/ICollisionBody.h"

namespace hvn3 {
	namespace System {

		CollisionBodyMutator::CollisionBodyMutator(ICollisionBody& body) : 
			_body(body) {}

		void CollisionBodyMutator::SetManager(ICollisionManager* manager) {

			_body._setManager(manager);

		}

	}
}