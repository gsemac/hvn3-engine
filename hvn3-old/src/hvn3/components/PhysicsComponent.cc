#include "hvn3/components/PhysicsComponent.h"

namespace hvn3 {

	// Public methods

	PhysicsComponent::PhysicsComponent() :
		_type(PhysicsBodyType::Dynamic),
		_mass(1.0f) {
	}

	const Vector2d& PhysicsComponent::LinearVelocity() const {
		return _velocity;
	}
	Vector2d& PhysicsComponent::LinearVelocity() {
		return _velocity;
	}
	void PhysicsComponent::SetLinearVelocity(const Vector2d& velocity) {
		_velocity = velocity;
	}
	void PhysicsComponent::SetLinearVelocity(float x, float y) {
		_velocity = Vector2d(x, y);
	}

	const PhysicsMaterial& PhysicsComponent::Material() const {
		return _material;
	}
	void PhysicsComponent::SetMaterial(const PhysicsMaterial& material) {
		_material = material;
	}

	float PhysicsComponent::Mass() const {
		return _mass;
	}
	float PhysicsComponent::InverseMass() const {
		return _mass != 0.0f ? (1.0f / _mass) : 0.0f;
	}
	void PhysicsComponent::SetMass(float mass) {
		_mass = mass;
	}

	PhysicsBodyType PhysicsComponent::Type() const {
		return _type;
	}
	void PhysicsComponent::SetType(PhysicsBodyType type) {
		_type = type;
	}

}