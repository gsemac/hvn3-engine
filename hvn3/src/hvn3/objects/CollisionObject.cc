#include "hvn3/objects/CollisionObject.h"
#include "hvn3/collision/ICollisionManager.h"

namespace hvn3 {

	CollisionObject::CollisionObject(ObjectId id, float x, float y) :
		Object(id, PointF(x, y)),
		_collision_body(this) {
	}
	CollisionObject::CollisionObject(ObjectId id, const PointF& position) :
		CollisionObject(id, position.X(), position.Y()) {
	}

	void CollisionObject::OnCreate(CreateEventArgs& e) {

		e.Collisions().AddBody(_collision_body);

		Object::OnCreate(e);

	}

	ICollisionBody& CollisionObject::CollisionBody() {

		return _collision_body;

	}
	const ICollisionBody& CollisionObject::CollisionBody() const {

		return _collision_body;

	}

}