#include "BasicPhysicsManager.h"
#include "Object.h"
#include <utility>

namespace hvn3 {
	namespace Physics {

		BasicPhysicsManager::BasicPhysicsManager(ICollisionManager* collision_manager) :
			_gravity(270.0f, 9.81f) {

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
				
				Object* obj = i->first;
				BasicPhysicsBody* body = &i->second;
				
				if (body->Type() != BodyType::Dynamic)
					continue;

				float x = obj->X();
				float y = obj->Y();

				_collision_manager->MoveContactIf(obj, _gravity.Angle(), _gravity.Magnitude(), 
					[](Object* obj) { return HasFlag(obj->Flags(), ObjectFlags::Solid); }
				);

			}

		}

	}
}