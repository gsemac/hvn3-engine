#pragma once
#include "IPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsBody : public IPhysicsBody {

		public:
			BasicPhysicsBody();

			const Vector2d& LinearVelocity() const override;
			void SetLinearVelocity(const Vector2d& vec) override;

			float Mass() const override;
			void SetMass(float value) override;

			virtual PhysicsBodyType Type() const override;
			virtual void SetType(PhysicsBodyType type) override;

		private:
			float _mass;
			Vector2d _linear_velocity;
			PhysicsBodyType _type;

		};

	}
}