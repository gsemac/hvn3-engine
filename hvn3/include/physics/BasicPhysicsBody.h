#pragma once
#include "IPhysicsBody.h"
#include "objects/Object.h"
#include <memory>

namespace hvn3 {

	class ICollisionBody;

	namespace Physics {

		class BasicPhysicsBody : public IPhysicsBody {

		public:
			BasicPhysicsBody(ICollisionBody* body);

			const PointF& Position() const override;
			void SetPosition(const PointF& position) override;
			const Vector2d& LinearVelocity() const override;
			void SetLinearVelocity(const Vector2d& vec) override;

			float Mass() const override;
			float InverseMass() const override;
			Physics::MassData MassData() const override;
			void SetMassData(const Physics::MassData& value) override;
			float Restitution() const override;
			void SetRestitution(float value) override;
			Vector2d Force() const override;
			void SetForce(const Vector2d& force) override;
			void ApplyForce(const Vector2d& force) override;
			void ApplyForce(const Vector2d& force, const PointF& point) override;
			Physics::Material Material() const override;
			void SetMaterial(const Physics::Material& material) override;

			virtual BodyType Type() const override;
			virtual void SetType(BodyType type) override;

		private:
			Physics::MassData _mass_data;
			float _restitution;
			mutable PointF _position;
			Vector2d _linear_velocity;
			BodyType _type;
			ICollisionBody* _body;
			Vector2d _force;
			Physics::Material _material;

			//Object* _key;
			//std::weak_ptr<Object> _object;

		};

	}
}