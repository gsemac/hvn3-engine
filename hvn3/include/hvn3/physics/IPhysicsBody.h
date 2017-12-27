#pragma once
#include "hvn3/math/Vector2d.h"
#include "hvn3/physics/Material.h"

namespace hvn3 {

	class ICollisionBody;
	class IPhysicsManager;
	class CategoryFilter;

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

		enum class BodyType {
			// A body that is not affected by world forces or collisions, and cannot be moved.
			Static,
			// A body that is not affected by world forces or collisions, but can be moved.
			Kinematic,
			// A body that is affected by world forces and reacts to collisions.
			Dynamic
		};

		class IPhysicsBody {
			friend class System::PhysicsBodyMutator;

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
			virtual Material Material() const = 0;
			virtual void SetMaterial(const Physics::Material& material) = 0;
			virtual BodyType Type() const = 0;
			virtual void SetType(BodyType type) = 0;
			virtual ICollisionBody& CollisionBody() = 0;
			virtual ICollisionBody& CollisionBody() const = 0;
			virtual CategoryFilter& Category() = 0;
			virtual const CategoryFilter& Category() const = 0;

		private:
			virtual void _setManager(IPhysicsManager* manager) = 0;

		};

	}
}