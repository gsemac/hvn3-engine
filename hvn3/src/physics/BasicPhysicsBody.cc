#include "physics/BasicPhysicsBody.h"
#include "collision/ICollisionBody.h"
#include "exceptions/Exception.h"

namespace hvn3 {
	namespace Physics {

		BasicPhysicsBody::BasicPhysicsBody(ICollisionBody* body) :
			_material(0.0f, 0.0f),
			_linear_velocity(0.0f, 0.0f) {

			//_key = object.get();
			//_object = object;

			_restitution = 0.0f;
			_type = BodyType::Dynamic;
			_body = body;
			SetMaterial(Material::Metal);

		}

		const PointF& BasicPhysicsBody::Position() const {

			_position.SetX(_body->X());
			_position.SetY(_body->Y());

			return _position;

		}
		void BasicPhysicsBody::SetPosition(const PointF& position) {

			_body->SetPosition(position);

		}
		const Vector2d&	BasicPhysicsBody::LinearVelocity() const {

			return _linear_velocity;

		}
		void BasicPhysicsBody::SetLinearVelocity(const Vector2d& vec) {

			_linear_velocity = vec;

		}

		float BasicPhysicsBody::Mass() const {

			return _mass_data.Mass;

		}
		float BasicPhysicsBody::InverseMass() const {

			return _mass_data.InverseMass;

		}
		Physics::MassData BasicPhysicsBody::MassData() const {

			return _mass_data;

		}
		void BasicPhysicsBody::SetMassData(const Physics::MassData& value) {

			_mass_data = value;

		}
		float BasicPhysicsBody::Restitution() const {

			return _restitution;

		}
		void BasicPhysicsBody::SetRestitution(float value) {

			_restitution = value;

		}
		Vector2d BasicPhysicsBody::Force() const {

			return _force;

		}
		void BasicPhysicsBody::SetForce(const Vector2d& force) {

			_force = force;

		}
		void BasicPhysicsBody::ApplyForce(const Vector2d& force) {

			_force += force;

		}
		void BasicPhysicsBody::ApplyForce(const Vector2d& force, const PointF& point) {

			throw System::NotImplementedException();

		}
		Physics::Material BasicPhysicsBody::Material() const {

			return _material;

		}
		void BasicPhysicsBody::SetMaterial(const Physics::Material& material) {

			_material = material;

			// mass = density * volume
			_mass_data.Mass = _material.Density * (32 * 32);
			_mass_data.InverseMass = _mass_data.Mass > 0.0f ? 1.0f / _mass_data.Mass : 0.0f;

		}

		BodyType BasicPhysicsBody::Type() const {

			return _type;


		}
		void BasicPhysicsBody::SetType(BodyType type) {

			_type = type;

		}

	}
}