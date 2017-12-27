#include "hvn3/math/MathUtils.h"
#include "hvn3/physics/IPhysicsBody.h"
#include "hvn3/physics/PhysicsUtils.h"

namespace hvn3 {
	namespace Physics {

		float StandardGravity() {

			return 9.80665f;

		}

		IntegrationResult GetEulerIntegrationResult(const IPhysicsBody& body, double delta) {

			Vector2d acceleration = body.Force() / body.Mass();
			Vector2d velocity = body.LinearVelocity() + (acceleration * delta);
			PointF position = body.Position() + (velocity * delta);

			return{ velocity, position };

		}
		IntegrationResult GetVerletIntegrationResult(const IPhysicsBody& body, const Vector2d& last_acceleration, double delta) {

			PointF position = body.Position();
			position += body.LinearVelocity() * delta + (0.5f * last_acceleration * delta * delta);

			Vector2d average_acceleration = ((body.Force() / body.Mass()) + last_acceleration) / 2.0f;
			Vector2d velocity = average_acceleration * delta;

			return{ velocity, position };

		}

		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other) {

			VelocityAfterCollisionResult result;
			float e = Math::Min(body.Restitution(), other.Restitution());

			result.velocity1 = body.LinearVelocity() * ((body.Mass() - e * other.Mass()) / (body.Mass() + other.Mass())) + other.LinearVelocity() * (e + 1.0f) * (other.Mass() / (body.Mass() + other.Mass()));
			result.velocity2 = body.LinearVelocity() * (e + 1.0f) * (body.Mass() / (body.Mass() + other.Mass())) + other.LinearVelocity() * ((other.Mass() - e * body.Mass()) / (body.Mass() + other.Mass()));

			return result;

		}
		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other, const Vector2d& normal) {

			VelocityAfterCollisionResult result = GetLinearVelocityAfterCollision(body, other);

			result.velocity1.SetDirection(normal.Direction());

			return result;

		}

	}
}