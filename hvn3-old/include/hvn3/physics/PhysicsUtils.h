#pragma once

#include "hvn3/collision/CollisionInfo.h"
#include "hvn3/components/PhysicsComponent.h"
#include "hvn3/components/TransformComponent.h"
#include "hvn3/math/MathUtils.h"
#include "hvn3/math/Point2d.h"

#include <cmath>

namespace hvn3 {

	namespace Physics {
		class IPhysicsBody;
	}

	// Returns the standard acceleration due to gravity on Earth in m/s. 
	float StandardGravity();

	struct IntegrationResult {
		Vector2d velocity;
		PointF position;
	};

	struct VelocityAfterCollisionResult {
		Vector2d velocity1;
		Vector2d velocity2;
	};

	Vector2d GetRelativeVelocity(const Vector2d& velocity1, const Vector2d& velocity2) {
		return velocity2 - velocity1;
	}
	float GetRelativeVelocityAlongNormal(const Vector2d& velocity1, const Vector2d& velocity2, const Vector2d normal) {
		return GetRelativeVelocity(velocity1, velocity2).DotProduct(normal);
	}
	Vector2d GetSeparationImpulse(const PhysicsComponent& body1, const PhysicsComponent& body2, const CollisionInfo& collisionInfo) {

		float e = Math::Min(body1.Material().Restitution(), body2.Material().Restitution());
		float impulse_scalar = -(1.0f + e) * GetRelativeVelocityAlongNormal(body1.LinearVelocity(), body2.LinearVelocity(), collisionInfo.Normal());

		impulse_scalar /= body1.InverseMass() + body2.InverseMass();

		Vector2d impulse = impulse_scalar * collisionInfo.Normal();

		return impulse;

	}

	void ResolveCollision(PhysicsComponent& body1, PhysicsComponent& body2, const CollisionInfo& collisionInfo) {

		if (GetRelativeVelocityAlongNormal(body1.LinearVelocity(), body2.LinearVelocity(), collisionInfo.Normal()) <= 0.0f) {

			// Only resolve the collision if bodies are moving towards each other.

			Vector2d impulse = GetSeparationImpulse(body1, body2, collisionInfo);

			body1.LinearVelocity() -= body1.InverseMass() * impulse;
			body2.LinearVelocity() -= body2.InverseMass() * impulse;

		}

	}
	void ApplyPositionalCorrection(const PhysicsComponent& body1, TransformComponent& transform1, const PhysicsComponent& body2, TransformComponent& transform2, const CollisionInfo& collisionInfo) {

		const float percentage_fix = 0.2f;
		const float sink_allowance = 0.01f;

		float correction_scalar = std::max(collisionInfo.PenetrationDepth() - sink_allowance, 0.0f) / (body1.InverseMass() + body2.InverseMass()) * percentage_fix;
		Vector2d correction = correction_scalar * collisionInfo.Normal();

		transform1.SetPosition(transform1.Position() -= body1.InverseMass() * correction);
		transform2.SetPosition(transform2.Position() += body2.InverseMass() * correction);

	}

	// Returns the next position and velocity of the body according to the Euler method.
	IntegrationResult GetEulerIntegrationResult(const Physics::IPhysicsBody& body, float delta);
	// Returns the next position and velocity of the body according to the Verlet method.
	IntegrationResult GetVerletIntegrationResult(const Physics::IPhysicsBody& body, const Vector2d& last_acceleration, float delta);

	// Returns the new linear velocities for both bodies upon collision.
	VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const Physics::IPhysicsBody& body, const Physics::IPhysicsBody& other);
	// Returns the new linear velocities for both bodies upon collision with respect to the collision normal.
	VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const Physics::IPhysicsBody& body, const Physics::IPhysicsBody& other, const Vector2d& normal);
	// Returns the impulse after a collision between the two bodies, without consideration for angular velocity.
	Vector2d GetLinearImpulseAfterCollision(const Physics::IPhysicsBody& body, const Physics::IPhysicsBody& other, const Vector2d& normal);

	// Returns the current acceleration of the body.
	Vector2d GetLinearAcceleration(const Physics::IPhysicsBody& body);
	// Returns the current momentum of the body.
	Vector2d GetLinearMomentum(const Physics::IPhysicsBody& body);

}