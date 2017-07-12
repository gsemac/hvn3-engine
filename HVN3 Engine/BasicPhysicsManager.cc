#include "BasicPhysicsManager.h"
#include "ICollisionBody.h"
#include "CollisionManifold.h"
#include "Direction8.h"
#include "PhysicsUtils.h"
#include "Object.h"
#include <utility>

namespace hvn3 {
	namespace Physics {

		BasicPhysicsManager::BasicPhysicsManager(Collision::ICollisionManager<Object*>* collision_manager) {

			_gravity = Vector2d::FromDirection(DIRECTION_DOWN, Physics::StandardGravity());
			_collision_manager = collision_manager;

		}

		BasicPhysicsBody* BasicPhysicsManager::GetBody(key_type key) {

			return const_cast<this_type*>(this)->GetBody(key);

		}
		const BasicPhysicsBody* BasicPhysicsManager::GetBody(key_type key) const {

			collection_type::const_iterator it = _bodies.find(key);

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

			std::cout << pairs.size();

			for (collection_type::iterator i = _bodies.begin(); i != _bodies.end(); ++i) {

				BasicPhysicsBody* body = &i->second;

			

				if (body->Type() != BodyType::Dynamic)
					continue;
				
				/*	_collision_manager->MoveContactIf(obj, _gravity.Direction(), _gravity.Length(),
						[](ICollisionBody* body) { return body->IsSolid(); }
					);*/

			}

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
			float j = -(1 + e) * velocity_along_normal;
			j /= 1 / body_1->Mass() + 1 / body_2->Mass();

			// Apply the impulse to both bodies.
			Vector2d impulse = manifold.Normal * j;
			body_1->SetLinearVelocity(body_1->LinearVelocity() - (1 / body_1->Mass() * impulse));
			body_2->SetLinearVelocity(body_2->LinearVelocity() + (1 / body_2->Mass() * impulse));

		}

	}
}