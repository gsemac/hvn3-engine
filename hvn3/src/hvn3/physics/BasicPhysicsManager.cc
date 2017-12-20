#include "hvn3/collision/CollisionManifold.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/physics/BasicPhysicsManager.h"
#include "hvn3/physics/IPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		void BasicPhysicsManager::AddBody(IPhysicsBody& body) {

			_body_lookup_table[&body.CollisionBody()] = &body;

			PhysicsManagerBase::AddBody(body);


		}
		void BasicPhysicsManager::RemoveBody(IPhysicsBody& body) {

			_body_lookup_table.erase(&body.CollisionBody());

			PhysicsManagerBase::RemoveBody(body);

		}

		void BasicPhysicsManager::OnUpdate(UpdateEventArgs& e) {

			for (auto i = Bodies().begin(); i != Bodies().end(); ++i) {

				IPhysicsBody* physics_body = (*i);
				ICollisionBody* collision_body = &(*i)->CollisionBody();

				collision_body->MoveContactIf(Gravity().Direction(), Gravity().Length(), [&](ICollisionBody* other) {

					IPhysicsBody* other_physics_body = _lookupBody(other);

					if (other_physics_body == nullptr)
						return false;

					return physics_body->Category().CheckHit(other_physics_body->Category());

				});


			}

			PhysicsManagerBase::OnUpdate(e);

		}



		IPhysicsBody* BasicPhysicsManager::_lookupBody(ICollisionBody* key) {

			auto it = _body_lookup_table.find(key);

			if (it == _body_lookup_table.end())
				return nullptr;

			return it->second;

		}

	}
}