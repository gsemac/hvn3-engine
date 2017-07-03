#pragma once
#include "Vector2d.h"

namespace hvn3 {
	namespace Physics {

		struct MassData {
			float Mass;
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

		public:
			virtual const Vector2d& LinearVelocity() const = 0;
			virtual void SetLinearVelocity(const Vector2d& vec) = 0;

			virtual float Mass() const = 0;
			virtual void SetMass(float value) = 0;
			
			virtual BodyType Type() const = 0;
			virtual void SetType(BodyType type) = 0;

		};

	}
}