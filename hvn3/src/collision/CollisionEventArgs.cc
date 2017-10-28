#include "collision/CollisionEventArgs.h"

namespace hvn3 {

	CollisionEventArgs::CollisionEventArgs(hvn3::Object* other, Collision::ICollisionBody* body) :
		_other(other),
		_body(body) {}

	Object* CollisionEventArgs::Object() {

		return _other;

	}
	const hvn3::Object* CollisionEventArgs::Object() const {

		return _other;

	}
	Collision::ICollisionBody* CollisionEventArgs::Body() {

		return _body;

	}
	Collision::ICollisionBody* CollisionEventArgs::Body() const {

		return _body;

	}

}