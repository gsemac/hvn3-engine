#pragma once
#include "IPhysicsManager.h"
#include "BasicPhysicsBody.h"
#include "ICollisionManager.h"
#include <unordered_map>

namespace hvn3 {
	
	class Object;
	class ICollisionBody;
	
	namespace Physics {

		class BasicPhysicsManager : public IPhysicsManager<ICollisionBody*> {

			typedef std::unordered_map<IPhysicsManager::key_type, BasicPhysicsBody> collection_type;
			typedef BasicPhysicsManager this_type;
			typedef BasicPhysicsBody mapped_type;

		public:
			BasicPhysicsManager(ICollisionManager<key_type>* collision_manager);

			BasicPhysicsBody* GetBody(key_type key) override;
			const BasicPhysicsBody* GetBody(key_type key) const override;
			BasicPhysicsBody* CreateBody(key_type key) override;

			const Vector2d& Gravity() const override;
			void SetGravity(const Vector2d& value) override;

			void OnUpdate(UpdateEventArgs& e) override;

		private:
			ICollisionManager<key_type>* _collision_manager;
			collection_type _bodies;
			Vector2d _gravity;

		};

	}
}