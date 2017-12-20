#include "hvn3/physics/PhysicsBodyMutator.h"
#include "hvn3/physics/IPhysicsBody.h"

namespace hvn3 {
	namespace System {

		PhysicsBodyMutator::PhysicsBodyMutator(Physics::IPhysicsBody& body) :
			_body(body) {}

		void PhysicsBodyMutator::SetManager(Physics::IPhysicsManager* manager) {

			_body._setManager(manager);

		}

	}
}