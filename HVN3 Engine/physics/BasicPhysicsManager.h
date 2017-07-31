#pragma once
#include "IPhysicsManager.h"
#include "BasicPhysicsBody.h"
#include "ICollisionManager.h"
#include <unordered_map>

namespace hvn3 {

	class Object;

	namespace Collision {
		class ICollisionBody;
		struct CollisionManifold;
	}

	namespace Physics {

		class BasicPhysicsManager : public IPhysicsManager<Collision::ICollisionBody*> {

			typedef std::unordered_map<IPhysicsManager::key_type, BasicPhysicsBody> collection_type;
			typedef BasicPhysicsManager* this_type;
			typedef BasicPhysicsBody mapped_type;

		public:
			BasicPhysicsManager(Collision::ICollisionManager<Object*>* collision_manager);

			BasicPhysicsBody* GetBody(key_type key) override;
			const BasicPhysicsBody* GetBody(key_type key) const override;
			BasicPhysicsBody* CreateBody(key_type key) override;

			const Vector2d& Gravity() const override;
			void SetGravity(const Vector2d& value) override;

			void OnUpdate(UpdateEventArgs& e) override;

		private:
			Collision::ICollisionManager<Object*>* _collision_manager;
			collection_type _bodies;
			Vector2d _gravity;	

			void _resolveCollision(IPhysicsBody* body_1, IPhysicsBody* body_2, Collision::CollisionManifold& manifold) const;
			void _integrateForces(IPhysicsBody* body, float dt) const;
			void _integrateVelocity(IPhysicsBody* body, float dt) const;
			void _positionalCorrection(IPhysicsBody* body_1, IPhysicsBody* body_2, Collision::CollisionManifold& manifold) const;

		};

	}
}