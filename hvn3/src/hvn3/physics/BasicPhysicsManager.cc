#include "hvn3/collision/CollisionManifold.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/physics/BasicPhysicsManager.h"
#include "hvn3/physics/IPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		void BasicPhysicsManager::AddBody(IPhysicsBody& body) {

			PhysicsManagerBase::AddBody(body);

			_body_lookup_table[&body.CollisionBody()] = &body;

		}
		void BasicPhysicsManager::RemoveBody(IPhysicsBody& body) {

			PhysicsManagerBase::RemoveBody(body);

			_body_lookup_table.erase(&body.CollisionBody());

		}

		void BasicPhysicsManager::OnUpdate(UpdateEventArgs& e) {

			PhysicsManagerBase::OnUpdate(e);
			
			for (auto i = Bodies().begin(); i != Bodies().end(); ++i) {

				IPhysicsBody* physics_body = (*i);
				ICollisionBody* collision_body = &(*i)->CollisionBody();

				if (physics_body->Type() == BodyType::Static)
					continue;

				Vector2d gravity_vector = physics_body->LinearVelocity() + Gravity() * PixelsToMetersScale();

				if (collision_body->PlaceFree(collision_body->X(), collision_body->Y() + 0.1f))
					physics_body->SetLinearVelocity(physics_body->LinearVelocity() + Gravity() * PixelsToMetersScale());
				else {

					// The body is contact with the floor. Stop all positive vertical velocity.
					if (physics_body->LinearVelocity().Y() > 0.0f)
						physics_body->SetLinearVelocity(Vector2d(physics_body->LinearVelocity().X(), 0.0f));

					// Reduce horizontal velocity according to friction. #todo
					physics_body->SetLinearVelocity(Vector2d(physics_body->LinearVelocity().X() * 0.8f, physics_body->LinearVelocity().Y()));

				}

				if (collision_body->MoveContactIf(physics_body->LinearVelocity().Direction(), physics_body->LinearVelocity().Length(), [&](ICollisionBody* other) {

					IPhysicsBody* other_physics_body = _lookupBody(other);

					if (other_physics_body != nullptr && physics_body->Category().CheckHit(other_physics_body->Category()))
						return true;

					return false;

				})) {

					//physics_body->SetLinearVelocity(Vector2d(physics_body->LinearVelocity().X(), 0.0f));

				}


			}

		}



		IPhysicsBody* BasicPhysicsManager::_lookupBody(ICollisionBody* key) {

			auto it = _body_lookup_table.find(key);

			if (it == _body_lookup_table.end())
				return nullptr;

			return it->second;

		}

	}
}