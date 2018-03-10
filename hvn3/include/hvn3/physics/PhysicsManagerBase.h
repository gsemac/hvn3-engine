#pragma once
#include "hvn3/core/UpdateEventArgs.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/physics/PhysicsUtils.h"
#include <list>

namespace hvn3 {
	namespace Physics {

		template <typename physics_body_type>
		class PhysicsManagerBase : public IPhysicsManager {

		public:
			typedef physics_body_type physics_body_type;

			PhysicsManagerBase() :
				_gravity(0.0f, Physics::StandardGravity()) {
				SetPixelsToMetersScale(1.0f / 32.0f);
			}

			IPhysicsBody* CreateBody(ICollisionBody* body) override {
				_bodies.push_back(physics_body_type(body));
				return &_bodies.back();
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
			size_t Count() const override {
				return _bodies.size();
			}
			void Clear() override {
				_bodies.clear();
			}

			void OnUpdate(UpdateEventArgs& e) override {}

		protected:
			typedef std::list<physics_body_type> physics_body_list_type;

			physics_body_list_type& GetBodies() {
				return _bodies;
			}
			const physics_body_list_type& Bodies() const {
				return _bodies;
			}

		private:
			physics_body_list_type _bodies;
			Vector2d _gravity;
			float _pixels_to_meters_scale;

		};

	}
}