#include "BasicPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		BasicPhysicsBody::BasicPhysicsBody() {

			_mass = 1.0f;
			_type = PhysicsBodyType::Dynamic;

		}

		const Vector2d&	BasicPhysicsBody::LinearVelocity() const {

			return _linear_velocity;

		}
		void BasicPhysicsBody::SetLinearVelocity(const Vector2d& vec) {

			_linear_velocity = vec;

		}
		
		float BasicPhysicsBody::Mass() const {

			return _mass;

		}
		void BasicPhysicsBody::SetMass(float value) {

			_mass = value;

		}

		PhysicsBodyType BasicPhysicsBody::Type() const {

			return _type;


		}
		void BasicPhysicsBody::SetType(PhysicsBodyType type) {

			_type = type;

		}

	}
}