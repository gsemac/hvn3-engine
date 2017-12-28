#pragma once
#include "hvn3/math/Point2d.h"

namespace hvn3 {
	namespace Physics {

		class IPhysicsBody;

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

		// Returns the next position and velocity of the body according to the Euler method.
		IntegrationResult GetEulerIntegrationResult(const IPhysicsBody& body, float delta);
		// Returns the next position and velocity of the body according to the Verlet method.
		IntegrationResult GetVerletIntegrationResult(const IPhysicsBody& body, const Vector2d& last_acceleration, float delta);

		// Returns the new linear velocities for both bodies upon collision.
		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other);
		// Returns the new linear velocities for both bodies upon collision with respect to the collision normal.
		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other, const Vector2d& normal);
		// Returns the impulse after a collision between the two bodies, without consideration for angular velocity.
		Vector2d GetLinearImpulseAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other, const Vector2d& normal);

		// Returns the current acceleration of the body.
		Vector2d GetLinearAcceleration(const IPhysicsBody& body);
		// Returns the current momentum of the body.
		Vector2d GetLinearMomentum(const IPhysicsBody& body);


	}
}