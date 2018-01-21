#include "hvn3/objects/PhysicsObject.h"
#include "hvn3/physics/IPhysicsManager.h"

namespace hvn3 {

	PhysicsObject::PhysicsObject(ObjectId id, float x, float y) :
		CollisionObject(id, PointF(x, y)),
		_physics_body(CollisionBody()) {
	}
	PhysicsObject::PhysicsObject(ObjectId id, const PointF& position) :
		PhysicsObject(id, position.X(), position.Y()) {
	}
	
	void PhysicsObject::OnCreate(CreateEventArgs& e) {

		e.Physics().AddBody(_physics_body);

		Object::OnCreate(e);

	}

	Physics::IPhysicsBody& PhysicsObject::PhysicsBody() {

		return _physics_body;

	}
	const Physics::IPhysicsBody& PhysicsObject::PhysicsBody() const {

		return _physics_body;

	}

}