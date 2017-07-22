#include "BasicPhysicsManager.h"
#include "ICollisionBody.h"
#include "CollisionManifold.h"
#include "Direction8.h"
#include "PhysicsUtils.h"
#include "Object.h"
#include "UpdateEventArgs.h"
#include <utility>

namespace hvn3 {
	namespace Physics {

		BasicPhysicsManager::BasicPhysicsManager(Collision::ICollisionManager<Object*>* collision_manager) {

			_gravity = Vector2d::FromDirection(DIRECTION_DOWN, Physics::StandardGravity() * 10.0f);
			_collision_manager = collision_manager;

		}

		BasicPhysicsBody* BasicPhysicsManager::GetBody(key_type key) {

			return const_cast<BasicPhysicsBody*>(static_cast<const BasicPhysicsManager*>(this)->GetBody(key));

		}
		const BasicPhysicsBody* BasicPhysicsManager::GetBody(key_type key) const {

			auto it = _bodies.find(key);

			if (it == _bodies.end())
				return nullptr;

			return &it->second;


		}
		BasicPhysicsBody* BasicPhysicsManager::CreateBody(key_type key) {

			auto it = _bodies.insert(std::pair<key_type, mapped_type>(key, BasicPhysicsBody(key)));

			return &it.first->second;

		}
		const Vector2d& BasicPhysicsManager::Gravity() const {

			return _gravity;

		}
		void BasicPhysicsManager::SetGravity(const Vector2d& value) {

			_gravity = value;

		}
		void BasicPhysicsManager::OnUpdate(UpdateEventArgs& e) {

			// If we do not have a collision manager, do nothing.
			if (_collision_manager == nullptr)
				return;
			
			// Get a list of colliding pairs from the collision manager.
			auto pairs = _collision_manager->CollidingPairs();
			
			// Integrate forces for each physics body.
			for (collection_type::iterator i = _bodies.begin(); i != _bodies.end(); ++i)
				_IntegrateForces(&i->second, e.Delta());

			// We need to handle the collision between each pair of bodies.
			for (auto i = pairs.begin(); i != pairs.end(); ++i) {

				// Get the physics body associated with each collision body.
				BasicPhysicsBody* body_a = GetBody(i->BodyA);
				BasicPhysicsBody* body_b = GetBody(i->BodyB);

				// If either are null, don't consider the collision.
				if (body_a == nullptr || body_b == nullptr)
					break;
				
				// Resolve the collsion.
				_ResolveCollision(body_a, body_b, *i);

			}

			// Integrate velocities for each physics body.
			for (collection_type::iterator i = _bodies.begin(); i != _bodies.end(); ++i)
				_IntegrateVelocity(&i->second, e.Delta());

		}

		// Private methods

		void BasicPhysicsManager::_ResolveCollision(IPhysicsBody* body_1, IPhysicsBody* body_2, Collision::CollisionManifold& manifold) const {

			// Calculate relative velocity.
			Vector2d relative_velocity = body_2->LinearVelocity() - body_1->LinearVelocity();

			// Calculate relative velocity in terms of the normal.
			float velocity_along_normal = relative_velocity.DotProduct(manifold.Normal);
			
			// If the velocities are separating, don't do anything.
			if (velocity_along_normal > 0.0f)
				return;
		
			// Use the lesser restitution of the two bodies.
			float e = Math::Min(body_1->Restitution(), body_2->Restitution());

			// Calculate the impulse scalar.
			float j = -(1.0f + e) * velocity_along_normal;
			j /= body_1->InverseMass() + body_2->InverseMass();

			// Apply the impulse to both bodies.
			Vector2d impulse = manifold.Normal * j;
		
			body_1->SetLinearVelocity(body_1->LinearVelocity() - (body_1->InverseMass() * impulse));
			body_2->SetLinearVelocity(body_2->LinearVelocity() + (body_2->InverseMass() * impulse));

		}
		void BasicPhysicsManager::_IntegrateForces(IPhysicsBody* body, float dt) const {

			if (body->InverseMass() == 0.0f)
				return;

			body->SetLinearVelocity(body->LinearVelocity() + (body->Force() * body->MassData().InverseMass + _gravity) * (dt / 2.0f));

		}
		void BasicPhysicsManager::_IntegrateVelocity(IPhysicsBody* body, float dt) const {

			if (body->InverseMass() == 0.0f)
				return;
		
			body->SetPosition(body->Position() + body->LinearVelocity() * dt);

		}

	}
}