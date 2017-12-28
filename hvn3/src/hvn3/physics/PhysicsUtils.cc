#include "hvn3/exceptions/Exception.h"
#include "hvn3/math/MathUtils.h"
#include "hvn3/physics/IPhysicsBody.h"
#include "hvn3/physics/PhysicsUtils.h"

namespace hvn3 {
	namespace Physics {

		float StandardGravity() {

			return 9.80665f;

		}

		IntegrationResult GetEulerIntegrationResult(const IPhysicsBody& body, float delta) {

			Vector2d acceleration = body.Force() / body.Mass();
			Vector2d velocity = body.LinearVelocity() + (acceleration * delta);
			PointF position = body.Position() + (velocity * delta);

			return{ velocity, position };

		}
		IntegrationResult GetVerletIntegrationResult(const IPhysicsBody& body, const Vector2d& last_acceleration, float delta) {

			PointF position = body.Position();
			position += body.LinearVelocity() * delta + (0.5f * last_acceleration * delta * delta);

			Vector2d average_acceleration = ((body.Force() / body.Mass()) + last_acceleration) / 2.0f;
			Vector2d velocity = average_acceleration * delta;

			return{ velocity, position };

		}

		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other) {

			VelocityAfterCollisionResult result;

			float e = 1.0f - Math::Min(body.Restitution(), other.Restitution());

			result.velocity1 = (body.Mass() * body.LinearVelocity() + other.Mass() * other.LinearVelocity() + other.Mass() * e * (other.LinearVelocity() - body.LinearVelocity())) / (body.Mass() + other.Mass());
			result.velocity2 = (body.Mass() * body.LinearVelocity() + other.Mass() * other.LinearVelocity() + body.Mass() * e * (body.LinearVelocity() - other.LinearVelocity())) / (body.Mass() + other.Mass());

			if (other.Type() == BodyType::Static)
				result.velocity1.SetDirection(180.0f - result.velocity1.Direction());
			else
				result.velocity1.SetDirection(result.velocity1.Direction() - 180.0f);

			return result;

		}
		VelocityAfterCollisionResult GetLinearVelocityAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other, const Vector2d& normal) {

			VelocityAfterCollisionResult result;
			Vector2d j = GetLinearImpulseAfterCollision(body, other, normal);
			float e = Math::Min(body.Restitution(), other.Restitution());

			result.velocity1 = body.LinearVelocity() + normal * (j * body.InverseMass()) * e;
			result.velocity2 = other.LinearVelocity() - normal * (j * other.InverseMass()) * e;

			return result;

		}
		Vector2d GetLinearImpulseAfterCollision(const IPhysicsBody& body, const IPhysicsBody& other, const Vector2d& normal) {

			float e = Math::Min(body.Restitution(), other.Restitution());

			return (-(1.0f + e) * (body.LinearVelocity() - other.LinearVelocity()) * normal) / (body.InverseMass() + other.InverseMass());

		}

		Vector2d GetLinearAcceleration(const IPhysicsBody& body) {

			return body.Force() / body.Mass();

		}
		Vector2d GetLinearMomentum(const IPhysicsBody& body) {

			return body.LinearVelocity() * body.Mass();

		}

	}
}