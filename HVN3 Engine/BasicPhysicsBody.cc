#include "BasicPhysicsBody.h"
#include "ICollisionBody.h"

namespace hvn3 {
	namespace Physics {

		BasicPhysicsBody::BasicPhysicsBody(Collision::ICollisionBody* body) {

			//_key = object.get();
			//_object = object;

			_mass = 1.0f;
			_restitution = 0.0f;
			_type = BodyType::Dynamic;
			_body = body;

		}

		const PointF& BasicPhysicsBody::Position() const {

			_position.SetX(_body->X());
			_position.SetY(_body->Y());

			return _position;

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
		float BasicPhysicsBody::Restitution() const {

			return _restitution;

		}
		void BasicPhysicsBody::SetRestitution(float value) {

			_restitution = value;

		}

		BodyType BasicPhysicsBody::Type() const {

			return _type;


		}
		void BasicPhysicsBody::SetType(BodyType type) {

			_type = type;

		}

	}
}