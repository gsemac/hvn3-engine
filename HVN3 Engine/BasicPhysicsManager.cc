#include "BasicPhysicsManager.h"
#include "ICollisionBody.h"
#include "Object.h"
#include <utility>

namespace hvn3 {
	namespace Physics {

		BasicPhysicsManager::BasicPhysicsManager(ICollisionManager<key_type>* collision_manager) {

			_gravity = Vector2d::FromDirection(270.0f, 9.81f);
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

			auto it = _bodies.insert(std::pair<key_type, mapped_type>(key, BasicPhysicsBody()));

			return &it.first->second;

		}

		const Vector2d& BasicPhysicsManager::Gravity() const {

			return _gravity;

		}
		void BasicPhysicsManager::SetGravity(const Vector2d& value) {

			_gravity = value;

		}

		void BasicPhysicsManager::OnUpdate(UpdateEventArgs& e) {

			for (collection_type::iterator i = _bodies.begin(); i != _bodies.end(); ++i) {
				
				ICollisionBody* obj = i->first;
				BasicPhysicsBody* body = &i->second;
				
				if (body->Type() != PhysicsBodyType::Dynamic)
					continue;

				_collision_manager->MoveContactIf(obj, _gravity.Angle(), _gravity.Magnitude(), 
					[](ICollisionBody* body) { return body->IsSolid(); }
				);

			}

		}

	}
}