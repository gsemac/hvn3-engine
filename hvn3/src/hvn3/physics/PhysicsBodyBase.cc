#include "hvn3/collision/ICollider.h"
#include "hvn3/physics/PhysicsBodyBase.h"
#include "hvn3/physics/IPhysicsManager.h"
#include "hvn3/exceptions/Exception.h"

namespace hvn3 {
	namespace Physics {

		PhysicsBodyBase::PhysicsBodyBase(ICollider* collision_body) :
			_material(),
			_linear_velocity(0.0f, 0.0f) {

			_type = BodyType::Dynamic;
			_collision_body = collision_body;
			_is_destroyed = false;

		}
		PhysicsBodyBase::~PhysicsBodyBase() {}

		const PointF PhysicsBodyBase::Position() const {

			return _collision_body->Position();

		}
		void PhysicsBodyBase::SetPosition(float x, float y) {

			_collision_body->SetPosition(x, y);

		}
		void PhysicsBodyBase::SetPosition(const PointF& position) {

			_collision_body->SetPosition(position);

		}
		const Vector2d&	PhysicsBodyBase::LinearVelocity() const {

			return _linear_velocity;

		}
		void PhysicsBodyBase::SetLinearVelocity(const Vector2d& vec) {

			if (_type != BodyType::Static)
				_linear_velocity = vec;

		}
		float PhysicsBodyBase::Mass() const {

			return _mass_data.Mass;

		}
		float PhysicsBodyBase::InverseMass() const {

			return _mass_data.InverseMass;

		}
		Physics::MassData PhysicsBodyBase::MassData() const {

			return _mass_data;

		}
		void PhysicsBodyBase::SetMassData(const Physics::MassData& value) {

			_mass_data = value;

		}
		float PhysicsBodyBase::Restitution() const {

			return _material.restitution;

		}
		void PhysicsBodyBase::SetRestitution(float value) {

			_material.restitution = value;

		}
		Vector2d PhysicsBodyBase::Force() const {

			return _force;

		}
		void PhysicsBodyBase::SetForce(const Vector2d& force) {

			_force = force;

		}
		void PhysicsBodyBase::ApplyForce(const Vector2d& force) {

			_force += force;

		}
		void PhysicsBodyBase::ApplyForce(const Vector2d& force, const PointF& point) {

			throw System::NotImplementedException();

		}
		Physics::Material PhysicsBodyBase::Material() const {

			return _material;

		}
		void PhysicsBodyBase::SetMaterial(const Physics::Material& material) {

			_material = material;

			_mass_data.Mass = _material.density * _collision_body->AABB().Area(); // mass = density * volume
			_mass_data.InverseMass = _mass_data.Mass > 0.0f ? 1.0f / _mass_data.Mass : 0.0f;

		}
		BodyType PhysicsBodyBase::Type() const {

			return _type;


		}
		void PhysicsBodyBase::SetType(BodyType type) {

			_type = type;

		}
		ICollider* PhysicsBodyBase::GetCollisionBody() {

			return _collision_body;

		}
		const ICollider* PhysicsBodyBase::CollisionBody() const {

			return _collision_body;

		}
		CategoryFilter& PhysicsBodyBase::GetCategory() {

			return _filter;

		}
		const CategoryFilter& PhysicsBodyBase::Category() const {

			return _filter;

		}

		void PhysicsBodyBase::Destroy() {
			_is_destroyed = true;
		}
		bool PhysicsBodyBase::IsDestroyed() const {
			return _is_destroyed;
		}
		void PhysicsBodyBase::OnDestroy(DestroyEventArgs& e) {}

	}
}