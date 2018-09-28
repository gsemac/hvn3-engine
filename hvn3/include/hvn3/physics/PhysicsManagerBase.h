#pragma once
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/physics/PhysicsUtils.h"

#include <list>
#include <unordered_map>

namespace hvn3 {
	namespace Physics {

		class PhysicsManagerBase :
			public IPhysicsManager {

		public:
			PhysicsManagerBase() :
				_gravity(0.0f, Physics::StandardGravity()) {

				SetPixelsToMetersScale(1.0f / 32.0f);

			}

			void Add(IPhysicsBodyPtr& body) override {

				_bodies.push_back(body);

				_collider_map[body->GetCollisionBody()] = body.get();

			}
			size_t Count() const override {
				return _bodies.size();
			}
			void Clear() override {
			
				_bodies.clear();
				_collider_map.clear();

			}

			const Vector2d& Gravity() const override {
				return _gravity;
			}
			void SetGravity(const Vector2d& value) override {
				_gravity = value;
			}
			float PixelsToMetersScale() const override {
				return _pixels_to_meters_scale;
			}
			void SetPixelsToMetersScale(float value) override {
				_pixels_to_meters_scale = value;
			}

			void OnUpdate(UpdateEventArgs& e) override {}

		protected:
			std::list<IPhysicsBodyPtr>& PhysicsBodies() {
				return _bodies;
			}
			const std::list<IPhysicsBodyPtr>& PhysicsBodies() const {
				return _bodies;
			}
			IPhysicsBody* GetPhysicsBodyFromCollider(const ICollider* collider) {

				auto it = _collider_map.find(collider);

				if (it == _collider_map.end())
					return nullptr;

				return it->second;

			}
			void ClearDeadPhysicsBodies() {

				PhysicsBodies().remove_if([&](IPhysicsBodyPtr& body) {

					bool dead = (body.use_count() <= 1);

					if (dead)
						_collider_map.erase(body->GetCollisionBody());

					return dead;

				});

			}

		private:
			std::list<IPhysicsBodyPtr> _bodies;
			std::unordered_map<const ICollider*, IPhysicsBody*> _collider_map;
			Vector2d _gravity;
			float _pixels_to_meters_scale;

		};

	}
}