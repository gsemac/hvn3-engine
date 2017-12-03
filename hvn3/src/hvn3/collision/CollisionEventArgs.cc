#include "hvn3/collision/CollisionEventArgs.h"

namespace hvn3 {

	CollisionEventArgs::CollisionEventArgs(hvn3::Object* other, ICollisionBody* body) :
		_other(other),
		_body(body) {}

	Object* CollisionEventArgs::Object() {

		return _other;

	}
	const hvn3::Object* CollisionEventArgs::Object() const {

		return _other;

	}
	ICollisionBody* CollisionEventArgs::Body() {

		return _body;

	}
	ICollisionBody* CollisionEventArgs::Body() const {

		return _body;

	}

}