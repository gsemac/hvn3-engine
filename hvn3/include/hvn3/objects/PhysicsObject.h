#pragma once
#include "hvn3/collision/ObjectCollisionBody.h"
#include "hvn3/objects/Object.h"
#include "hvn3/objects/ObjectTypeDefs.h"
#include "hvn3/physics/PhysicsBodyBase.h"

namespace hvn3 {

	class PhysicsObject : public Object {

	public:
		PhysicsObject(ObjectId id, float x, float y);
		PhysicsObject(ObjectId id, const PointF& position);

		void OnCreate(CreateEventArgs& e) override;

		ICollisionBody& CollisionBody();
		const ICollisionBody& CollisionBody() const;
		Physics::IPhysicsBody& PhysicsBody();
		const Physics::IPhysicsBody& PhysicsBody() const;

	private:
		ObjectCollisionBody _collision_body;
		Physics::PhysicsBodyBase _physics_body;

	};

}