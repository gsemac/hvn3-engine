#include "hvn3/collision/CollisionEventArgs.h"

namespace hvn3 {

	CollisionEventArgs::CollisionEventArgs(IObject* other, ICollisionBody* other_body) :
		_other(other),
		_other_body(other_body) {
	}

	ICollisionBody* CollisionEventArgs::Body() const {
		return _other_body;
	}
	const IObject* CollisionEventArgs::Object() const {
		return _other;
	}
	IObject* CollisionEventArgs::GetObject() {
		return _other;
	}
	ICollisionBody* CollisionEventArgs::GetBody() {
		return _other_body;
	}

}