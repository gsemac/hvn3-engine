#pragma once
#include "hvn3/collision/CollisionDefs.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/physics/PhysicsDefs.h"
#include "hvn3/physics/PhysicsMaterial.h"

namespace hvn3 {

	class ICollider;
	class IPhysicsManager;
	class CollisionFilter;

	namespace System {
		class PhysicsBodyMutator;
	}

	namespace Physics {

		struct MassData {
			MassData() {
				Mass = 1.0f;
				InverseMass = 1.0f;
				RotationalInertia = 0.0f;
				Center = PointF(0.0f, 0.0f);
			}
			float Mass;
			float InverseMass;
			float RotationalInertia;
			PointF Center;
		};

		class IPhysicsBody {

		public:
			virtual ~IPhysicsBody() = default;

			virtual const PointF Position() const = 0;
			virtual void SetPosition(float x, float y) = 0;
			virtual void SetPosition(const PointF& position) = 0;
			virtual const Vector2d& LinearVelocity() const = 0;
			virtual void SetLinearVelocity(const Vector2d& vec) = 0;
			virtual float Mass() const = 0;
			virtual float InverseMass() const = 0;
			virtual Physics::MassData MassData() const = 0;
			virtual void SetMassData(const Physics::MassData& value) = 0;
			virtual float Restitution() const = 0;
			virtual void SetRestitution(float value) = 0;
			virtual Vector2d Force() const = 0;
			virtual void SetForce(const Vector2d& force) = 0;
			virtual void ApplyForce(const Vector2d& force) = 0;
			virtual void ApplyForce(const Vector2d& force, const PointF& point) = 0;
			virtual PhysicsMaterial Material() const = 0;
			virtual void SetMaterial(const PhysicsMaterial& material) = 0;
			virtual PhysicsBodyType Type() const = 0;
			virtual void SetType(PhysicsBodyType type) = 0;
			virtual ICollider* GetCollisionBody() = 0;
			virtual const ICollider* CollisionBody() const = 0;
			virtual CollisionFilter& GetCategory() = 0;
			virtual const CollisionFilter& Category() const = 0;

		};

	}
}