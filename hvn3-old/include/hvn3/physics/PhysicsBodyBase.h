#pragma once
#include "hvn3/collision/CollisionFilter.h"
#include "hvn3/physics/IPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		class PhysicsBodyBase : 
			public IPhysicsBody {

		public:
			PhysicsBodyBase(IColliderPtr& collision_body);
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
			PhysicsMaterial Material() const override;
			void SetMaterial(const PhysicsMaterial& material) override;
			PhysicsBodyType Type() const override;
			void SetType(PhysicsBodyType type) override;
			ICollider* GetCollisionBody() override;
			const ICollider* CollisionBody() const override;
			CollisionFilter& GetCategory() override;
			const CollisionFilter& Category() const override;

		private:
			Physics::MassData _mass_data;
			Vector2d _linear_velocity;
			PhysicsBodyType _type;
			Vector2d _force;
			PhysicsMaterial _material;
			IColliderPtr _collision_body;
			CollisionFilter _filter;

		};

	}
}