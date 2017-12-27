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
		IntegrationResult GetEulerIntegrationResult(const IPhysicsBody& body, double delta);
		// Returns the next position and velocity of the body according to the Verlet method.
		IntegrationResult GetVerletIntegrationResult(const IPhysicsBody& body, const Vector2d& last_acceleration, double delta);

		// Returns the new linear velocities for both bodies when they collide.
		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other);
		// Returns the new linear velocities for both bodies when they collide with respect to the collision normal.
		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other, const Vector2d& normal);


	}
}