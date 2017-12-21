#include "hvn3/collision/ICollisionManager.h"
#include "hvn3/objects/PhysicsObject.h"
#include "hvn3/physics/IPhysicsManager.h"

namespace hvn3 {

	PhysicsObject::PhysicsObject(ObjectId id, float x, float y) :
		Object(id, PointF(x, y)),
		_collision_body(this),
		_physics_body(_collision_body) {
	}
	PhysicsObject::PhysicsObject(ObjectId id, const PointF& position) :
		PhysicsObject(id, position.X(), position.Y()) {
	}

	void PhysicsObject::OnCreate(CreateEventArgs& e) {

		e.Collisions().AddBody(_collision_body);
		e.Physics().AddBody(_physics_body);

		Object::OnCreate(e);

	}

	ICollisionBody& PhysicsObject::CollisionBody() {

		return _collision_body;

	}
	const ICollisionBody& PhysicsObject::CollisionBody() const {

		return _collision_body;

	}
	Physics::IPhysicsBody& PhysicsObject::PhysicsBody() {

		return _physics_body;

	}
	const Physics::IPhysicsBody& PhysicsObject::PhysicsBody() const {

		return _physics_body;

	}

}