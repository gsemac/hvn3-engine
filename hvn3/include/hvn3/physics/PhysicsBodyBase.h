#pragma once
#include "hvn3/collision/CategoryFilter.h"
#include "hvn3/physics/IPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		class PhysicsBodyBase : public IPhysicsBody {

		public:
			PhysicsBodyBase(CollisionBodyPtr& collision_body);
			~PhysicsBodyBase();

			virtual const PointF Position() const override;
			virtual void SetPosition(float x, float y) override;
			virtual void SetPosition(const PointF& position) override;
			const Vector2d& LinearVelocity() const override;
			void SetLinearVelocity(const Vector2d& vec) override;
			float Mass() const override;
			float InverseMass() const override;
			Physics::MassData MassData() const override;
			void SetMassData(const Physics::MassData& value) override;
			float Restitution() const override;
			void SetRestitution(float value) override;
			Vector2d Force() const override;
			void SetForce(const Vector2d& force) override;
			void ApplyForce(const Vector2d& force) override;
			void ApplyForce(const Vector2d& force, const PointF& point) override;
			Physics::Material Material() const override;
			void SetMaterial(const Physics::Material& material) override;
			BodyType Type() const override;
			void SetType(BodyType type) override;
			CollisionBodyPtr& CollisionBody() override;
			const CollisionBodyPtr& CollisionBody() const override;
			CategoryFilter& Category() override;
			const CategoryFilter& Category() const override;

		protected:
			IPhysicsManager* Manager();

		private:
			Physics::MassData _mass_data;
			Vector2d _linear_velocity;
			BodyType _type;
			Vector2d _force;
			Physics::Material _material;
			CollisionBodyPtr _collision_body;
			CategoryFilter _filter;
			IPhysicsManager* _manager;

			bool _managerIsSet() const;
			void _setManager(IPhysicsManager* manager) override;

		};

	}
}