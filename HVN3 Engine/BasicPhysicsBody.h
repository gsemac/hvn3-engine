#pragma once
#include "IPhysicsBody.h"
#include "Object.h"
#include <memory>

namespace hvn3 {
	namespace Physics {

		class BasicPhysicsBody : public IPhysicsBody {

		public:
			BasicPhysicsBody(ObjectPtr& object);

			const PointF& Position() const override;
			const Vector2d& LinearVelocity() const override;
			void SetLinearVelocity(const Vector2d& vec) override;

			float Mass() const override;
			void SetMass(float value) override;
			float Restitution() const override;
			void SetRestitution(float value) override;

			virtual BodyType Type() const override;
			virtual void SetType(BodyType type) override;

		private:
			float _mass;
			float _restitution;
			mutable PointF _position;
			Vector2d _linear_velocity;
			BodyType _type;

			Object* _key;
			std::weak_ptr<Object> _object;

		};

	}
}