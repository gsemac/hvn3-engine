#include "hvn3/collision/CollisionFilter.h"
#include "hvn3/collision/CollisionResult.h"
#include "hvn3/collision/ICollider.h"
#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/core/Context.h"
#include "hvn3/math/GeometryUtils.h"
#include "hvn3/physics/BasicPhysicsManager.h"
#include "hvn3/physics/IPhysicsBody.h"
#include "hvn3/physics/PhysicsUtils.h"

namespace hvn3 {
	namespace Physics {

		BasicPhysicsManager::BasicPhysicsManager() {}
		void BasicPhysicsManager::OnUpdate(UpdateEventArgs& e) {

			PhysicsManagerBase::OnUpdate(e);

			_contacts.clear();

			bool has_dead_bodies = false;

			for (auto i = PhysicsBodies().begin(); i != PhysicsBodies().end(); ++i) {

				if (i->use_count() <= 1) {
					has_dead_bodies = true;
					continue;
				}

				// Get the physics and collision bodies associated with this body.
				IPhysicsBody* this_body = i->get();
				ICollider* collision_body = this_body->GetCollisionBody();

				// If the body is static, we do not need to apply physics to it.
				if (this_body->Type() == BodyType::Static)
					continue;

				// Lambda for filtering collision bodies. Only returns true for bodies this body can collide with in the physics system.
				auto body_filter = [&](const ICollider* other) {
					IPhysicsBody* other_body = GetPhysicsBodyFromCollider(other);
					if (other_body != nullptr && this_body->Category().CheckMatch(other_body->Category()))
						return true;
					return false;
				};

				// Calculate and apply the force of gravity.
				Vector2d force_gravity = this_body->Mass() * Gravity() * PixelsToMetersScale();
				this_body->ApplyForce(force_gravity);

				// Calculate the new position and velocity.
				IntegrationResult integration_result = GetEulerIntegrationResult(*this_body, 1.0);
				this_body->SetLinearVelocity(integration_result.velocity);

				// If the current position isn't free, move until it is.
				// #todo Need to check condition as below
				CollisionResult manifold;
				if (!_context.Get<COLLISION_MANAGER>().PlaceFreeIf(collision_body, collision_body->Position(), manifold, body_filter)) {
					_context.Get<COLLISION_MANAGER>().MoveOutside(collision_body, Math::Geometry::PointDirection(manifold.collider->AABB().Midpoint(), collision_body->AABB().Midpoint()), Math::Max(2.0f, manifold.penetrationDepth / 2.0f));
				}

				// Move the body towards its new position until it hits something.
				Vector2d movement_vector = Vector2d(this_body->Position(), integration_result.position);
				if (_context.Get<COLLISION_MANAGER>().MoveContactIf(collision_body, movement_vector.Direction(), movement_vector.Length(), manifold, body_filter)) {

					// Resolve the collision with the other body.
					IPhysicsBody* other_body = GetPhysicsBodyFromCollider(manifold.collider);

					VelocityAfterCollisionResult result = GetLinearVelocityAfterCollision(*this_body, *other_body);

					// Bodies hit will have their velocities updated after all bodies have been updated for this step.
					_contacts.push_back(Contact{ other_body, result.velocity2 });

					result.velocity1.SetDirection(manifold.normal.Direction());
					this_body->SetLinearVelocity(result.velocity1);

				}

				this_body->SetForce(Vector2d(0.0f, 0.0f));

			}

			for (auto i = _contacts.begin(); i != _contacts.end(); ++i)
				i->body->SetLinearVelocity(i->velocity);

			// Clear all destroyed bodies from the system.

			if (has_dead_bodies)
				ClearDeadPhysicsBodies();

		}
		void BasicPhysicsManager::Clear() {

			PhysicsManagerBase::Clear();

			_contacts.clear();

		}

		void BasicPhysicsManager::OnContextChanged(ContextChangedEventArgs& e) {
			_context = e.Context();
		}

	}
}