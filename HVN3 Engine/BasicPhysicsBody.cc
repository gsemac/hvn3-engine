#include "BasicPhysicsBody.h"

namespace hvn3 {
	namespace Physics {

		BasicPhysicsBody::BasicPhysicsBody(ObjectPtr& object) {

			_key = object.get();
			_object = object;

			_mass = 1.0f;
			_restitution = 0.0f;
			_type = BodyType::Dynamic;

		}

		const PointF& BasicPhysicsBody::Position() const {

			_position.SetX(_key->X());
			_position.SetY(_key->Y());

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