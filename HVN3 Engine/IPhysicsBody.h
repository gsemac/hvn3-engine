#pragma once
#include "Vector2d.h"

namespace hvn3 {
	namespace Physics {

		struct MassData {
			float Mass;
			float RotationalInertia;
			PointF Center;
		};

		class IPhysicsBody {

		public:
			virtual const Vector2d& LinearVelocity() const = 0;
			virtual void SetLinearVelocity(const Vector2d& vec) = 0;

			virtual float Mass() const = 0;
			virtual void SetMass(float value) = 0;
			
		};

	}
}