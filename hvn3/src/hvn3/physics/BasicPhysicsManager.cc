#include "hvn3/collision/CollisionManifold.h"
#include "hvn3/collision/ICollisionBody.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/physics/BasicPhysicsManager.h"
#include "hvn3/physics/IPhysicsBody.h"
#include "hvn3/physics/PhysicsUtils.h"

namespace hvn3 {
	namespace Physics {

		struct contact {
			IPhysicsBody* a;
			IPhysicsBody* b;
			VelocityAfterCollisionResult result;
		};

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

			_contacts.clear();

			for (auto i = Bodies().begin(); i != Bodies().end(); ++i) {

				// Get the physics and collision bodies associated with this body.
				IPhysicsBody& this_body = *(*i);
				ICollisionBody& collision_body = (*i)->CollisionBody();

				// If the body is static, we do not need to apply physics to it.
				if (this_body.Type() == BodyType::Static)
					continue;

				// Lambda for filtering collision bodies. Only returns true for bodies this body can collide with in the physics system.
				auto body_filter = [&](ICollisionBody* other) {
					IPhysicsBody* other_body = _lookupBody(other);
					if (other_body != nullptr && this_body.Category().CheckHit(other_body->Category()))
						return true;
					return false;
				};

				// Calculate and apply the force of gravity.
				Vector2d force_gravity = this_body.Mass() * Gravity() * PixelsToMetersScale();
				this_body.ApplyForce(force_gravity);

				// Calculate the new position and velocity.
				IntegrationResult integration_result = GetEulerIntegrationResult(this_body, 1.0);
				this_body.SetLinearVelocity(integration_result.velocity);

				// If the current position isn't free, move until it is.
				// #todo Need to check condition as below
				CollisionManifold manifold;
				if (!e.Collisions().PlaceFreeIf(collision_body, collision_body.Position(), manifold, body_filter)) {
					e.Collisions().MoveOutside(collision_body, Math::Geometry::PointDirection(manifold.bodyB->AABB().Midpoint(), collision_body.AABB().Midpoint()), Math::Max(2.0f, manifold.penetrationDepth / 2.0f));
				}

				// Move the body towards its new position until it hits something.
				Vector2d movement_vector = Vector2d(this_body.Position(), integration_result.position);
				if (e.Collisions().MoveContactIf(collision_body, movement_vector.Direction(), movement_vector.Length(), manifold, body_filter)) {

					// Resolve the collision with the other body.
					IPhysicsBody* other_body = _lookupBody(manifold.bodyB);

					VelocityAfterCollisionResult result = GetLinearVelocityAfterCollision(this_body, *other_body);

					// Bodies hit will have their velocities updated after all bodies have been updated for this step.
					_contacts.push_back(Contact{ other_body, result.velocity2 });

					result.velocity1.SetDirection(manifold.normal.Direction());
					this_body.SetLinearVelocity(result.velocity1);
					
				}

				this_body.SetForce(Vector2d(0.0f, 0.0f));

			}

			for (auto i = _contacts.begin(); i != _contacts.end(); ++i)
				i->body->SetLinearVelocity(i->velocity);

		}



		IPhysicsBody* BasicPhysicsManager::_lookupBody(ICollisionBody* key) {

			auto it = _body_lookup_table.find(key);

			if (it == _body_lookup_table.end())
				return nullptr;

			return it->second;

		}

	}
}