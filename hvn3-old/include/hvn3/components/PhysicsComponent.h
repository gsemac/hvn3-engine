#pragma once

#include "hvn3/components/ColliderComponent.h"
#include "hvn3/components/TransformComponent.h"
#include "hvn3/ecs/ComponentBase.h"
#include "hvn3/math/Vector2d.h"
#include "hvn3/physics/PhysicsDefs.h"
#include "hvn3/physics/PhysicsMaterial.h"

namespace hvn3 {

	class PhysicsComponent :
		public ecs::ComponentBase<TypeList<ColliderComponent>> {

	public:
		PhysicsComponent();

		const Vector2d& LinearVelocity() const;
		Vector2d& LinearVelocity();
		void SetLinearVelocity(const Vector2d& velocity);
		void SetLinearVelocity(float x, float y);

		const PhysicsMaterial& Material() const;
		void SetMaterial(const PhysicsMaterial& material);

		float Mass() const;
		float InverseMass() const;
		void SetMass(float mass);

		PhysicsBodyType Type() const;
		void SetType(PhysicsBodyType type);

	private:
		Vector2d _velocity;
		PhysicsBodyType _type;
		PhysicsMaterial _material;
		float _mass;

	};

}